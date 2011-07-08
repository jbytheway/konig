#ifndef KONIG__ANNOUNCEMENT_HPP
#define KONIG__ANNOUNCEMENT_HPP

#include <konig/api.hpp>
#include <konig/feat.hpp>
#include <konig/announcedness.hpp>

namespace konig {

struct KONIG_API Announcement {
  friend class boost::serialization::access;
  public:
    Announcement() = default; // For serialization
    Announcement(Feat, Announcedness, bool defensive);
    static bool from_range(
      Announcement&,
      std::string::const_iterator& start,
      std::string::const_iterator const& finish
    );
    static bool from_string(Announcement&, std::string const&);
    static bool
    many_from_string(std::vector<Announcement>&, std::string const&);

    Feat feat;
    Announcedness announcedness;
    bool defensive;

    bool offensive() const { return !defensive; }
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(feat) &
        BOOST_SERIALIZATION_NVP(announcedness) &
        BOOST_SERIALIZATION_NVP(defensive);
    }
};

inline bool operator==(Announcement const& l, Announcement const& r) {
  return l.feat == r.feat &&
    l.announcedness == r.announcedness &&
    l.defensive == r.defensive;
}

std::ostream& operator<<(std::ostream&, Announcement const&);

}

#endif // KONIG__ANNOUNCEMENT_HPP

