#ifndef KONIG_CLIENT__SERVERINTERFACE_HPP
#define KONIG_CLIENT__SERVERINTERFACE_HPP

#include <set>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>
#include <messaging/send.hpp>

#include <konig/message.hpp>
#include <konig/protocol.hpp>
#include <konig/client/clientinterface.hpp>

namespace konig { namespace client {

class ServerInterface {
  public:
    ServerInterface(asio::io_service&, ClientInterface&);
    ~ServerInterface();

    ClientId id() const { return id_; }

    void error(
        const messaging::error_source,
        const boost::system::error_code&
      );
    template<typename Message, typename Connection>
    void message(const Message& m, Connection&) {
      message(m);
    }
#define KONIG_CLIENT_SERVERINTERFACE_MESSAGE(r, d, value) \
    void message(const Message<MessageType::value>&);
    BOOST_PP_SEQ_FOR_EACH(
        KONIG_CLIENT_SERVERINTERFACE_MESSAGE, _, KONIG_MESSAGETYPE_VALUES()
      )
#undef KONIG_CLIENT_SERVERINTERFACE_MESSAGE

    template<typename Message>
    void send(Message const& m) {
      messaging::send<konig::Protocol>(m, connection_);
    }

    void close();
  private:
    messaging::connection::ptr connection_;
    ClientInterface& client_;
    ClientId id_;
};

}}

#endif // KONIG_CLIENT__SERVERINTERFACE_HPP

