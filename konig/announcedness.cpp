#include <konig/announcedness.hpp>

#include <stdexcept>

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
      case Achievement::made:
        result += "+";
        break;
      default:
        throw std::logic_error("invalid achievement");
    }
  }

  return result;
}

}

