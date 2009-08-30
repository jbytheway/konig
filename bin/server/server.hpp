#ifndef KONIG_SERVER__SERVER_HPP
#define KONIG_SERVER__SERVER_HPP

#include <set>
#include <sstream>

#include <boost/asio/ip/tcp.hpp>

#include <messaging/callback_helper.hpp>
#include <messaging/server.hpp>

#include <konig/protocol.hpp>
#include "client.hpp"

namespace konig { namespace server {

class Server {
  private:
    typedef messaging::callback_helper<Server> callback_helper;
  public:
    Server(boost::asio::io_service& io, std::ostream&);

    template<typename Connection>
    void new_connection(Connection& c) {
      Client::Ptr client(new Client(c, *this));
      clients_.insert(client);
      out_ << "Added client\n";
    }

    void error(
        const messaging::error_source es,
        const boost::system::error_code& ec
      ) {
      std::ostringstream os;
      os << "server: " << es << ": " << ec.message();
      throw std::runtime_error(os.str());
    }

    void remove_client(const Client::Ptr&);
  private:
    std::ostream& out_;
    messaging::server<Protocol, callback_helper> message_server_;
    std::set<Client::Ptr> clients_;
};

}}

#endif // KONIG_SERVER__SERVER_HPP

