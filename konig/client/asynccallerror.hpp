#ifndef KONIG_CLIENT__ASYNCCALLERROR_HPP
#define KONIG_CLIENT__ASYNCCALLERROR_HPP

namespace konig { namespace client {

class AsyncCallError : std::runtime_error {
  public:
    AsyncCallError() :
      std::runtime_error("error in attempt to get info from user")
    {}
};

}}

#endif // KONIG_CLIENT__ASYNCCALLERROR_HPP

