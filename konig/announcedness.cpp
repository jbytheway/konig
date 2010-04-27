#include <konig/announcedness.hpp>

#include <stdexcept>

#include <konig/fatal.hpp>

namespace konig {

std::string Announcedness::string(const Achievement achievement) const
{
  std::string result;
  switch (value_) {
    case unannounced:
    case announced:
      break;
    case kontraed:
      result = "x";
      break;
    case rekontraed:
      result = "xx";
      break;
    case subkontraed:
      result = "xxx";
      break;
    default:
      throw std::logic_error("invalid announcedness");
  }
  if (value_ == unannounced) {
    if (achievement == Achievement::off) {
      result += "/";
    }
  } else {
    switch (achievement) {
      case  Achievement::off:
        result += "-";
        break;
      case Achievement::neutral:
        break;
      case Achievement::made:
        result += "+";
        break;
      default:
        throw std::logic_error("invalid achievement");
    }
  }

  return result;
}

int Announcedness::multiplier() const
{
  switch (value_) {
    case unannounced:
      KONIG_FATAL("no multiplier for unannounced");
    case announced:
      return 1;
    case kontraed:
      return 2;
    case rekontraed:
      return 4;
    case subkontraed:
      return 8;
    default:
      throw std::logic_error("invalid announcedness");
  }
}

}

