#ifndef KONIG_SERVER_REMOTECALLERROR_HPP
#define KONIG_SERVER_REMOTECALLERROR_HPP

#include <stdexcept>

namespace konig { namespace server {

class RemoteCallError : public std::runtime_error {
  public:
    RemoteCallError() :
      std::runtime_error("remote call failed")
    {}
};

}}

#endif // KONIG_SERVER_REMOTECALLERROR_HPP

