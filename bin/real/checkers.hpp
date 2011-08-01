#ifndef KONIG_REAL_CHECKERS_HPP
#define KONIG_REAL_CHECKERS_HPP

#include <boost/lexical_cast.hpp>

namespace konig { namespace real {

namespace checkers {

  class PlayPositionChecker : public terminal::Checker {
    public:
      typedef konig::PlayPosition return_type;

      PlayPositionChecker(boost::any& r) :
        terminal::Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& spos = tokens.front();
        int pos;
        try {
          pos = boost::lexical_cast<int>(spos);
        } catch (boost::bad_lexical_cast const&) {
          return false;
        }

        if (pos >= position_min && pos < position_max) {
          return_ = PlayPosition(pos);
          return true;
        }
        return false;
      }
  };

}

}}

#endif // KONIG_REAL_CHECKERS_HPP

