#ifndef KONIG_CLIENT__PLAYERINTERFACE_HPP
#define KONIG_CLIENT__PLAYERINTERFACE_HPP

namespace konig { namespace client {

class PlayerInterface {
  public:
    virtual void warning(const std::string&) = 0;
    virtual void abort() = 0;
};

}}

#endif // KONIG_CLIENT__PLAYERINTERFACE_HPP

