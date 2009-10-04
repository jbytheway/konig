#include "server.hpp"

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
class Server::Callbacks : st::branch_callback {
  public:
    Callbacks(Server& s) : server_(s) {}
    st::branch_callback& branch() { return *this; }
  private:
    Server& server_;
    virtual void children_altered(st::branch& altered) {
      server_.send_to_clients(
          Message<MessageType::notifySetting>(
            altered.name(), altered.child_names()
          )
        );
    }
};

Server::Server(boost::asio::io_service& io, std::ostream& o) :
  out_(o),
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
      st::make("clients", callbacks_->branch())
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

std::string Server::set_request(
    st::user& user,
    std::string const& address,
    std::string const& val
  )
{
  return settings_->change_request(address, val, user);
}

void Server::remove_client(Client const* client)
{
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

