#ifndef KONIG__FATAL_HPP
#define KONIG__FATAL_HPP

#include <cassert>
#include <ostream>

namespace konig {

extern std::ostream& error_stream;

}

#define KONIG_FATAL(msg) \
  do { \
    konig::error_stream << __FILE__ << ":" << __LINE__ << ":" << \
      __PRETTY_FUNCTION__ <<  ": fatal error: " << msg << std::endl; \
    abort(); \
  } while(false)

#endif // KONIG__FATAL_HPP

