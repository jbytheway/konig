#ifndef KONIG_PROTOCOL_HPP
#define KONIG_PROTOCOL_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <konig/message.hpp>

namespace konig {

struct Protocol {
  typedef boost::archive::xml_oarchive oarchive_type;
  typedef boost::archive::xml_iarchive iarchive_type;
  typedef MessageType::internal_enum message_index_type;
  static const MessageType::internal_enum max_message_index = MessageType::max;
  template<typename type_index>
  struct message_type_from_index {
    typedef Message<MessageType::internal_enum(type_index::value)> type;
  };

  static const uint16_t port = 4386;
};

}

#endif // KONIG_PROTOCOL_HPP

