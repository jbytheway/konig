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
    ServerInterface(asio::io_service&, ClientInterface&);
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
      client_.warning(os.str()); \
    }
    KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::setPlayerProperties)
#undef KONIG_CLIENT_SERVERINTERFACE_IGNORE

    void close();
  private:
    messaging::connection::ptr connection_;
    ClientInterface& client_;
};

}}

#endif // KONIG_CLIENT__SERVERINTERFACE_HPP

