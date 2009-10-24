#ifndef KONIG__ANNOUNCEMENT_HPP
#define KONIG__ANNOUNCEMENT_HPP

#include <konig/feat.hpp>
#include <konig/announcedness.hpp>

namespace konig {

struct Announcement {
  friend class boost::serialization::access;
  public:
    Announcement() = default; // For serialization
    Announcement(Feat, Announcedness, bool defensive);
    static bool from_string(Announcement&, std::string const&);

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

}

#endif // KONIG__ANNOUNCEMENT_HPP

