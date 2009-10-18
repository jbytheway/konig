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
#define KONIG_CLIENT_SERVERINTERFACE_IGNORE(type) \
    void message(const Message<type>&) {          \
      std::ostringstream os;                      \
      os << "warning: ignoring message of type " << type; \
      client_.warning(os.str());                  \
    }
    KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::getSetting)
    KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::setSetting)
    KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::bid)
#undef KONIG_CLIENT_SERVERINTERFACE_IGNORE
    void message(Message<MessageType::joined> const&);
    void message(Message<MessageType::rejection> const&);
    void message(Message<MessageType::notifySetting> const&);
    void message(Message<MessageType::startGame> const&);
    void message(Message<MessageType::requestBid> const&);

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

