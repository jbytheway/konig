#include "server.hpp"

#include <konig/fatal.hpp>

namespace konig { namespace server {

// Static stuff for coping with interrupts
namespace {
  bool interrupted = false;

  void interrupt_handler(int /*signal*/)
  {
    interrupted = true;
  }
}

Server::Server(boost::asio::io_service& io, std::ostream& o) :
  out_(o),
  message_server_(
      io,
      callback_helper(*this),
      asio::ip::tcp::endpoint(asio::ip::tcp::v4(), Protocol::port)
    ),
  interrupt_monitor_(io, boost::posix_time::milliseconds(10))
{
  signal(SIGINT, &interrupt_handler);
  signal(SIGTERM, &interrupt_handler);

  o << "Server started.  Hit Ctrl+C to interrupt and shut down server.\n";
  interrupt_monitor_.async_wait(boost::bind(
        &Server::check_for_interrupt, this, boost::asio::placeholders::error
      ));
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

void Server::remove_client(const Client::Ptr& client)
{
  clients_.erase(client);
}

void Server::close()
{
  out_ << "Server interrupted.  Shutting down" << std::endl;
  BOOST_FOREACH(Client::Ptr const& client, clients_) {
    client->close();
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

}}

