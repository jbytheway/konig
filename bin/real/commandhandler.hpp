#ifndef KONIG_REAL_COMMANDHANDLER_HPP
#define KONIG_REAL_COMMANDHANDLER_HPP

#include <konig/terminal/commandhandler.hpp>

namespace konig { namespace real {

class CommandHandler : public terminal::CommandHandler {
  public:
    CommandHandler(boost::asio::io_service&);
};

}}

#endif // KONIG_REAL_COMMANDHANDLER_HPP

