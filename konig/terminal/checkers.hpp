#ifndef KONIG_TERMINAL_CHECKERS_HPP
#define KONIG_TERMINAL_CHECKERS_HPP

#include <unordered_map>

#include <konig/terminal/checker.hpp>

namespace konig { namespace terminal {

namespace checkers {

  class BidChecker : public Checker {
    public:
      typedef Bid return_type;

      BidChecker(boost::any& r, Ruleset const& rules) :
        Checker(r),
        rules_(rules)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& token = tokens.front();
        if (token == "pass" || token == "p") {
          return_ = Bid::pass;
          return true;
        }
        Contracts const& contracts = rules_.contracts();
        Bid bid = contracts.index_by_bid_name(token);
        if (!bid.is_pass()) {
          return_ = bid;
          return true;
        }
        return false;
      }
    private:
      Ruleset const& rules_;
  };

  class KingChecker : public Checker {
    public:
      typedef KingCall return_type;

      KingChecker(boost::any& r) :
        Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& king = tokens.front();
        KingCall k;
        if (KingCall::from_string(k, king)) {
          return_ = k;
          return true;
        }
        return false;
      }
  };

  class BoolChecker : public Checker {
    public:
      typedef bool return_type;

      BoolChecker(boost::any& r) :
        Checker(r)
      {
        if (bools_.empty()) {
          bools_ = {
            {"y", true }, {"yes", true}, {"1", true },
            {"n", false}, {"no", false}, {"0", false}
          };
        }
      }

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& value = tokens.front();
        auto it = bools_.find(value);
        if (it == bools_.end()) {
          return false;
        }
        return_ = it->second;
        return true;
      }
    private:
      static std::unordered_map<std::string, bool> bools_;
  };

  std::unordered_map<std::string, bool> BoolChecker::bools_;

  class TalonChecker : public Checker {
    public:
      typedef uint8_t return_type;

      TalonChecker(boost::any& r) :
        Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& choice = tokens.front();
        if (choice == "0") {
          return_ = uint8_t(0);
          return true;
        } else if (choice == "1") {
          return_ = uint8_t(1);
          return true;
        }
        return false;
      }
  };

  class CardsChecker : public Checker {
    public:
      typedef Cards return_type;

      CardsChecker(boost::any& r) :
        Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        Cards cards;
        BOOST_FOREACH(auto const& token, tokens) {
          Cards thisCards;
          if (Cards::from_string(thisCards, token)) {
            cards.insert(thisCards);
          } else {
            return false;
          }
        }
        return_ = cards;
        return true;
      }
  };

  class AnnounceChecker : public Checker {
    public:
      typedef std::vector<Announcement> return_type;

      AnnounceChecker(boost::any& r) :
        Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.empty()) return false;
        std::vector<Announcement> announcements;
        if (tokens.size() == 1 &&
            (tokens.back() == "p" || tokens.back() == "pass")) {
          return_ = announcements;
          return true;
        }
        BOOST_FOREACH(auto const& token, tokens) {
          Announcement announcement;
          if (Announcement::from_string(announcement, token)) {
            announcements.push_back(announcement);
          } else {
            return false;
          }
        }
        return_ = announcements;
        return true;
      }
  };

  class CardChecker : public Checker {
    public:
      typedef Card return_type;

      CardChecker(boost::any& r) :
        Checker(r)
      {}

      virtual bool command(std::list<std::string> const& tokens) {
        if (tokens.size() != 1) return false;
        std::string const& token = tokens.front();
        Card card;
        if (Card::from_string(card, token)) {
          return_ = card;
          return true;
        }
        return false;
      }
  };

}

}}

#endif // KONIG_TERMINAL_CHECKERS_HPP

