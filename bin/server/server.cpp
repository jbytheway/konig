#include "server.hpp"

#include <array>

#include <settingstree/make.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace server {

namespace st = settingstree;

// Static stuff for coping with interrupts
namespace {
  bool interrupted = false;

  void interrupt_handler(int /*signal*/)
  {
    interrupted = true;
  }
}

// Definition of private Callbacks class (pimpl'd into Server)
class Server::Callbacks :
  st::branch_callback,
  st::leaf_callback<bool>,
  st::leaf_callback<std::string> {
  public:
    Callbacks(Server& s) : server_(s) {}
    st::branch_callback& branch() { return *this; }
    st::leaf_callback<std::string>& ai() { return *this; }
    st::leaf_callback<bool>& go() { return *this; }
  private:
    Server& server_;

    // For ai
    virtual std::string setting_altering(st::string_leaf& l, std::string value)
    {
      if (server_.ai_exe_.empty()) {
        return "no executable for running AI";
      }
      TablePosition table_pos(boost::lexical_cast<int>(l.name()));
      server_.io_.post(
          std::bind(&Server::reset_ai, &server_, table_pos, std::move(value))
        );
      return "";
    }

    // For go
    virtual std::string setting_altering(st::bool_leaf&, bool value)
    {
      if (!value) {
        return "cannot unset go";
      }
      return server_.test_go();
    }

    // For branch
    virtual void children_altered(st::branch& altered) {
      server_.send_to_clients(
          Message<MessageType::notifySetting>(
            altered.full_name(), altered.child_names()
          )
        );
    }

    // For all leaves
    virtual void setting_altered(st::leaf& altered) {
      server_.notify_setting(altered);
    }
};

Server::Server(
    boost::asio::io_service& io,
    std::ostream& o,
    boost::filesystem::path ai_exe
  ) :
  io_(io),
  out_(o),
  ai_exe_(std::move(ai_exe)),
  message_server_(
      io,
      callback_helper(*this),
      asio::ip::tcp::endpoint(asio::ip::tcp::v4(), Protocol::port)
    ),
  interrupt_monitor_(io, boost::posix_time::milliseconds(10)),
  callbacks_(new Callbacks(*this))
{
  signal(SIGINT, &interrupt_handler);
  signal(SIGTERM, &interrupt_handler);

  o << "Server started.  Hit Ctrl+C to interrupt and shut down server.\n";
  interrupt_monitor_.async_wait(boost::bind(
        &Server::check_for_interrupt, this, boost::asio::placeholders::error
      ));

  settings_ =
    st::make("", callbacks_->branch(),
      st::make("clients", callbacks_->branch()),
      st::make("server", callbacks_->branch(),
        st::make("go", callbacks_->go(), false, "admin")
      ),
      st::make("ai", callbacks_->branch(),
        st::make("1", callbacks_->ai(), "", "admin"),
        st::make("2", callbacks_->ai(), "", "admin"),
        st::make("3", callbacks_->ai(), "", "admin"),
        st::make("4", callbacks_->ai(), "", "admin")
      )
    ).tree_ptr();
}

Server::~Server()
{
  signal(SIGINT, SIG_DFL);
  signal(SIGTERM, SIG_DFL);
}

void Server::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  ) {
  std::ostringstream os;
  os << "server: " << es << ": " << ec.message();
  if (!interrupted) {
    throw std::runtime_error(os.str());
  }
}

void Server::warning(std::string const& w)
{
  out_ << "warning: " << w << std::endl;
}

boost::tuple<std::string, std::set<std::string>, st::node const*>
Server::get_request(st::user& user, std::string const& address)
{
  return settings_->get_request(address, user);
}

std::string Server::set_request(
    st::user& user,
    std::string const& address,
    std::string const& val
  )
{
  return settings_->change_request(address, val, user);
}

void Server::add_client(std::unique_ptr<Client> client)
{
  // Careful! Be sure to get id before moving from client
  ClientId id = client->id();
  auto& clientsBranch =
    dynamic_cast<st::branch&>(settings_->get_node("clients"));
  std::string const group = "client"+id.to_string();
  clientsBranch.add_child(
      st::make(id.to_string(), callbacks_->branch(),
        st::make("position", client->callback_position(), uint8_t(0), group)
      ).node_ptr(clientsBranch)
    );
  clients_[id] = std::move(client);
  out_ << "Added client\n";
}

void Server::remove_client(Client const* client)
{
  auto& clientsBranch =
    dynamic_cast<st::branch&>(settings_->get_node("clients"));
  clientsBranch.remove_child(client->id().to_string());
  clients_.erase(client->id());
}

void Server::close()
{
  out_ << "Server interrupted.  Shutting down" << std::endl;
  BOOST_FOREACH(Clients::value_type const& client_pair, clients_) {
    client_pair.second->close();
  }
  message_server_.close_listeners();
}

void Server::notify_setting(st::leaf& altered)
{
  send_to_clients(
      Message<MessageType::notifySetting>(
        altered.full_name(), altered.value_set()
      )
    );
}

void Server::reset_ai(TablePosition pos, std::string args)
{
  std::string spos = boost::lexical_cast<std::string>(int(pos));
  ai_[pos].reset(
      ai_exe_,
      boost::assign::list_of(std::string("-p"))(spos)("-a")(std::move(args))
    );
}

std::string Server::test_go()
{
  std::array<std::set<ClientId>, TablePosition::max> clients_in_positions;
  BOOST_FOREACH(Clients::value_type const& p, clients_) {
    Client const& client = *p.second;
    clients_in_positions[client.table_position()].insert(client.id());
  }
  // Note we don't care about clients in position 0
  for (int i=1; i<TablePosition::max; ++i) {
    if (clients_in_positions[i].size() != 1) {
      return "Not exactly one client in position "+
        boost::lexical_cast<std::string>(i);
    }
  }
  io_.post(boost::bind(&Server::go, this));
  return "";
}

void Server::go()
{
  KONIG_FATAL("not implemented");
}

void Server::check_for_interrupt(boost::system::error_code const& e)
{
  if (e) {
    KONIG_FATAL("error in Server::check_for_interrupt");
  } else if (interrupted) {
    close();
  } else {
    interrupt_monitor_.expires_from_now(boost::posix_time::milliseconds(10));
    interrupt_monitor_.async_wait(boost::bind(
          &Server::check_for_interrupt, this, boost::asio::placeholders::error
        ));
  }
}

ClientId Server::free_client_id() const
{
  // If we really cared about performance this could be faster, but I hope it's
  // irrelevant
  ClientId i;
  do {
    if (!clients_.count(i)) {
      return i;
    }
    ++i;
  } while (i.valid());

  throw std::runtime_error("too many clients");
}

template<typename Message>
void Server::send_to_clients(Message const& m)
{
  BOOST_FOREACH(Clients::value_type const& client_pair, clients_) {
    client_pair.second->send(m);
  }
}

}}

