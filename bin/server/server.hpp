#ifndef KONIG_SERVER__SERVER_HPP
#define KONIG_SERVER__SERVER_HPP

#include <set>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <messaging/callback_helper.hpp>
#include <messaging/server.hpp>

#include <settingstree/tree.hpp>
#include <settingstree/user.hpp>

#include <konig/protocol.hpp>
#include "client.hpp"

namespace konig { namespace server {

class Server {
  private:
    typedef messaging::callback_helper<Server> callback_helper;
  public:
    Server(boost::asio::io_service& io, std::ostream&);
    ~Server();

    template<typename Connection>
    void new_connection(Connection& c) {
      Client::Ptr client(new Client(c, *this, free_client_id()));
      clients_[client->id()] = client;
      out_ << "Added client\n";
    }

    void error(
        const messaging::error_source es,
        const boost::system::error_code& ec
      );

    void warning(std::string const&);

    std::string set_request(
        settingstree::user&,
        std::string const& address,
        std::string const& val
      );
    void remove_client(const Client::Ptr&);
    void close();
  private:
    void check_for_interrupt(boost::system::error_code const&);

    ClientId free_client_id() const;

    // Templated but private so can go in cpp file
    template<typename Message>
    void send_to_clients(Message const& m);

    std::ostream& out_;
    messaging::server<Protocol, callback_helper> message_server_;
    boost::unordered_map<ClientId, Client::Ptr> clients_;
    boost::asio::deadline_timer interrupt_monitor_;
    settingstree::tree::ptr settings_;

    struct Callbacks;
    std::unique_ptr<Callbacks> callbacks_;
};

}}

#endif // KONIG_SERVER__SERVER_HPP

