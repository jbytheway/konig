#ifndef KONIG_DTERM__ASYNCCALLERROR_HPP
#define KONIG_DTERM__ASYNCCALLERROR_HPP

namespace konig { namespace dterm {

class AsyncCallError : std::runtime_error {
  public:
    AsyncCallError() :
      std::runtime_error("error in attempt to get info from user")
    {}
};

}}

#endif // KONIG_DTERM__ASYNCCALLERROR_HPP

