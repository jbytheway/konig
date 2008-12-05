#ifndef KONIG_CLIENT__SERVERINTERFACE_HPP
#define KONIG_CLIENT__SERVERINTERFACE_HPP

#include <set>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>

#include <konig/message.hpp>
#include <konig/client/clientinterface.hpp>

namespace konig { namespace client {

class ServerInterface {
  public:
    ServerInterface(asio::io_service&);
    ~ServerInterface();

    void error(
        const messaging::error_source,
        const boost::system::error_code&
      );
    template<typename Message, typename Connection>
    void message(const Message& m, Connection&) {
      message(m);
    }
#define KONIG_CLIENT_SERVERINTERFACE_IGNORE(type) \
    void message(const Message<type>&) {          \
      std::ostringstream os;                      \
      os << "warning: ignoring message of type " << type; \
      for_each_player(boost::bind(&ClientInterface::warning, _1, os.str())); \
    }
    KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::setPlayerProperties)
#undef KONIG_CLIENT_SERVERINTERFACE_IGNORE
  private:
    template<typename UnaryFunction>
    void for_each_player(const UnaryFunction& f) {
      std::for_each(players_.begin(), players_.end(), f);
    }

    messaging::connection::ptr connection_;
    std::set<ClientInterface*> players_;
};

}}

#endif // KONIG_CLIENT__SERVERINTERFACE_HPP

