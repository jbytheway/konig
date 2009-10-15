#ifndef KONIG_SERVER__SERVER_HPP
#define KONIG_SERVER__SERVER_HPP

#include <array>
#include <set>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <messaging/callback_helper.hpp>
#include <messaging/server.hpp>

#include <settingstree/tree.hpp>
#include <settingstree/user.hpp>

#include <konig/protocol.hpp>
#include "client.hpp"
#include "process.hpp"

namespace konig { namespace server {

class Server {
  private:
    typedef messaging::callback_helper<Server> callback_helper;
  public:
    Server(
        boost::asio::io_service& io,
        std::ostream&,
        boost::filesystem::path ai_exe
      );
    ~Server();

    template<typename Connection>
    void new_connection(Connection& c) {
      std::unique_ptr<Client> client(new Client(c, *this, free_client_id()));
      add_client(std::move(client));
    }

    void error(
        const messaging::error_source es,
        const boost::system::error_code& ec
      );

    void warning(std::string const&);

    boost::tuple<std::string, std::set<std::string>, settingstree::node const*>
    get_request(settingstree::user& user, std::string const& address);

    std::string set_request(
        settingstree::user&,
        std::string const& address,
        std::string const& val
      );
    void add_client(std::unique_ptr<Client>);
    void remove_client(Client const*);
    void close();

    void notify_setting(settingstree::leaf&);
    void reset_ai(TablePosition, std::string args);
    std::string test_go();
  private:
    void go();
    void check_for_interrupt(boost::system::error_code const&);

    ClientId free_client_id() const;

    // Templated but private so can go in cpp file
    template<typename Message>
    void send_to_clients(Message const& m);

    boost::asio::io_service& io_;
    std::ostream& out_;
    boost::filesystem::path ai_exe_;
    messaging::server<Protocol, callback_helper> message_server_;
    typedef boost::unordered_map<ClientId, std::unique_ptr<Client>> Clients;
    Clients clients_;
    boost::asio::deadline_timer interrupt_monitor_;
    settingstree::tree::ptr settings_;
    std::array<Process, TablePosition::max> ai_;

    struct Callbacks;
    std::unique_ptr<Callbacks> callbacks_;
};

}}

#endif // KONIG_SERVER__SERVER_HPP

