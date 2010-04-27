#include <konig/featvalues.hpp>

#include <konig/fatal.hpp>

namespace konig {

int FeatValues::value_of(Feat f, bool announced) const
{
  auto it = value_map_.find(std::make_pair(f, announced));
  if (it == value_map_.end()) {
    KONIG_FATAL("invalid feat");
  }
  return it->second;
}

FeatValues FeatValues::cheltenham()
{
  ValueMap map;
  map.insert(std::make_pair(std::make_pair(Feat::pagat, false), 1));
  map.insert(std::make_pair(std::make_pair(Feat::pagat, true), 2));
  map.insert(std::make_pair(std::make_pair(Feat::uhu, false), 1));
  map.insert(std::make_pair(std::make_pair(Feat::uhu, true), 3));
  map.insert(std::make_pair(std::make_pair(Feat::kakadu, false), 1));
  map.insert(std::make_pair(std::make_pair(Feat::kakadu, true), 4));
  map.insert(std::make_pair(std::make_pair(Feat::king_ultimo, false), 1));
  map.insert(std::make_pair(std::make_pair(Feat::king_ultimo, true), 2));
  map.insert(std::make_pair(std::make_pair(Feat::forty_five, false), 0));
  map.insert(std::make_pair(std::make_pair(Feat::forty_five, true), 2));
  map.insert(std::make_pair(std::make_pair(Feat::valat, false), 4));
  map.insert(std::make_pair(std::make_pair(Feat::valat, true), 8));
  return FeatValues(map);
}

FeatValues FeatValues::operator*(int r)
{
  ValueMap map;
  BOOST_FOREACH(auto const& p, value_map_) {
    map.insert(std::make_pair(p.first, p.second*r));
  }
  return FeatValues(map);
}

FeatValues::FeatValues(ValueMap map) :
  value_map_(std::move(map))
{}

}

