#ifndef KONIG_FATAL_HPP
#define KONIG_FATAL_HPP

#include <cassert>
#include <ostream>

#include <konig/api.hpp>

namespace konig {

extern KONIG_API std::ostream& error_stream;

}

#define KONIG_FATAL(msg) \
  do { \
    konig::error_stream << __FILE__ << ":" << __LINE__ << ":" << \
      __PRETTY_FUNCTION__ <<  ": fatal error: " << msg << std::endl; \
    abort(); \
  } while(false)

#endif // KONIG_FATAL_HPP

