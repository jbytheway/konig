#ifndef KONIG_CLIENT__CLIENTINTERFACE_HPP
#define KONIG_CLIENT__CLIENTINTERFACE_HPP

#include <konig/player.hpp>
#include <konig/clientid.hpp>

namespace konig { namespace client {

class ClientInterface {
  public:
    virtual void message(std::string const&) = 0;
    virtual void warning(std::string const&) = 0;
    virtual void abort() = 0;
    virtual Player& player() = 0;
    virtual void id_known(ClientId) {}
};

}}

#endif // KONIG_CLIENT__CLIENTINTERFACE_HPP

