#ifndef KONIG_AI__INVALIDERROR_HPP
#define KONIG_AI__INVALIDERROR_HPP

namespace konig { namespace ai {

class KONIG_AI_API InvalidError : public std::logic_error {
  public:
    InvalidError(std::string const& m) :
      std::logic_error(m)
    {}
};

class KONIG_AI_API InvalidPlayError : public InvalidError {
  public:
    InvalidPlayError(std::string const& m) :
      InvalidError(m)
    {}
};

class KONIG_AI_API InvalidAnnouncementsError : public InvalidError {
  public:
    InvalidAnnouncementsError(std::string const& m) :
      InvalidError(m)
    {}
};

}}

#endif // KONIG_AI__INVALIDERROR_HPP

