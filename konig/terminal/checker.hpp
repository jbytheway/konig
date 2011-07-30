#ifndef KONIG_TERMINAL_CHECKER_HPP
#define KONIG_TERMINAL_CHECKER_HPP

#include <list>

#include <boost/any.hpp>

namespace konig { namespace terminal {

// Classes inheriting from Checker provide a first-line check on commands to
// see if they are suitable for the currently required game mechanic (e.g. a
// bid)
class Checker {
  public:
    Checker(boost::any& r);
    virtual bool command(std::list<std::string> const& tokens) = 0;
  protected:
    boost::any& return_;
};

}}

#endif // KONIG_TERMINAL_CHECKER_HPP

