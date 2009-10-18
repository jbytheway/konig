#ifndef KONIG_SERVER__REMOTE_CALL_ERROR_HPP
#define KONIG_SERVER__REMOTE_CALL_ERROR_HPP

#include <stdexcept>

namespace konig { namespace server {

class remote_call_error : public std::runtime_error {
  public:
    remote_call_error() :
      std::runtime_error("remote call failed")
    {}
};

}}

#endif // KONIG_SERVER__REMOTE_CALL_ERROR_HPP

