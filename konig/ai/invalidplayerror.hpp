#ifndef KONIG_AI__INVALIDPLAYERROR_HPP
#define KONIG_AI__INVALIDPLAYERROR_HPP

namespace konig { namespace ai {

class InvalidPlayError : public std::logic_error {
  public:
    InvalidPlayError(std::string const& m) :
      std::logic_error(m)
    {}
};

}}

#endif // KONIG_AI__INVALIDPLAYERROR_HPP

