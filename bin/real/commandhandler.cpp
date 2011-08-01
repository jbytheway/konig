#include "commandhandler.hpp"

namespace konig { namespace real {

CommandHandler::CommandHandler(boost::asio::io_service& io) :
  terminal::CommandHandler(io, "> ")
{}

}}

