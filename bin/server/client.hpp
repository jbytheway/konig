#ifndef KONIG_SERVER_CLIENT_HPP
#define KONIG_SERVER_CLIENT_HPP

#include <typeinfo>

#include <boost/shared_ptr.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/any.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>
#include <messaging/callback_helper.hpp>
#include <messaging/send.hpp>

#include <settingstree/user.hpp>
#include <settingstree/leaf_callback.hpp>

#include <konig/fatal.hpp>
#include <konig/message.hpp>
#include <konig/protocol.hpp>
#include <konig/clientid.hpp>
#include <konig/tableposition.hpp>

#include "remotecallerror.hpp"

namespace konig { namespace server {

class Server;

class Client : private settingstree::user {
  public:
    template<typename Connection>
    Client(
        boost::asio::io_service& io,
        Connection& c,
        Server& s,
        ClientId id
      ) :
      settingstree::user("client"+id.to_string(), "admin"),
      io_(io),
      id_(id),
      table_position_(0),
      server_(s),
      connection_(c.shared_from_this()),
      aborting_(NULL),
      callbacks_(*this),
      expected_remote_return_type_(NULL)
    {
      c.reset_callbacks(messaging::callback_helper<Client>(*this));
      send(Message<MessageType::joined>(id));
    }

    ~Client();

    ClientId id() const { return id_; }
    TablePosition table_position() const { return table_position_; }

    template<typename Message, typename Connection>
    void message(const Message& m, Connection&) {
      message(m);
    }

#define KONIG_SERVER_CLIENT_MESSAGE(r, d, value) \
    void message(const Message<MessageType::value>&);
    BOOST_PP_SEQ_FOR_EACH(
        KONIG_SERVER_CLIENT_MESSAGE, _, KONIG_MESSAGETYPE_VALUES()
      )
#undef KONIG_SERVER_CLIENT_MESSAGE
    void error(const messaging::error_source, const boost::system::error_code&);

    template<typename Message>
    void send(Message const& m) {
      messaging::send<konig::Protocol>(m, connection_);
    }

    template<
      MessageType::internal_enum request,
      MessageType::internal_enum response
    >
    typename Message<response>::only_value remote_call();

    void close();

    settingstree::leaf_callback<std::uint8_t>& callback_position();
    std::string set_table_position(TablePosition);
  private:
    boost::asio::io_service& io_;
    ClientId id_;
    TablePosition table_position_;
    Server& server_;
    messaging::connection::ptr connection_;
    bool* aborting_;

    class Callbacks : settingstree::leaf_callback<std::uint8_t> {
      public:
        Callbacks(Client& c) : client_(c) {}
        settingstree::leaf_callback<std::uint8_t>& position();
      private:
        virtual std::string setting_altering(
            settingstree::int_leaf<std::uint8_t>&,
            std::uint8_t
          );
        virtual void setting_altered(settingstree::leaf&);
        Client& client_;
    };
    Callbacks callbacks_;

    std::type_info const* expected_remote_return_type_;
    boost::any remote_return_value_;
};

template<
  MessageType::internal_enum request,
  MessageType::internal_enum response
>
typename Message<response>::only_value Client::remote_call()
{
  typedef typename Message<response>::only_value ReturnType;
  if (expected_remote_return_type_ ||
      !remote_return_value_.empty() ||
      aborting_) {
    KONIG_FATAL("re-entrant use of remote_call");
  }
  expected_remote_return_type_ = &typeid(ReturnType);
  bool aborting = false;
  aborting_ = &aborting;
  send(Message<request>());
  do {
    io_.run_one();
    if (aborting) {
      throw RemoteCallError();
    }
  } while (remote_return_value_.empty());
  ReturnType returnValue = boost::any_cast<ReturnType>(remote_return_value_);
  aborting_ = NULL;
  expected_remote_return_type_ = NULL;
  remote_return_value_ = boost::any();
  return returnValue;
}

}}

#endif // KONIG_SERVER_CLIENT_HPP

