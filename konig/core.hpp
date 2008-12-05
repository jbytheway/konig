#ifndef KONIG__CORE_HPP
#define KONIG__CORE_HPP

#include <boost/spirit/home/phoenix/core/argument.hpp>

namespace boost {
  namespace asio {}
  namespace fusion {}
}

namespace konig {
  namespace asio = boost::asio;
  namespace fusion = boost::fusion;
  namespace px = boost::phoenix;
  
  using px::arg_names::arg1;
  using px::arg_names::arg2;
  using px::arg_names::arg3;
}

#endif // KONIG__CORE_HPP

