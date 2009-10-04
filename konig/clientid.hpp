#ifndef KONIG__CLIENTID_HPP
#define KONIG__CLIENTID_HPP

#include <cstdint>

#include <konig/utility/idbase.hpp>

namespace konig {

class ClientId : public utility::IdBase<std::uint8_t, ClientId> {
};

}

#endif // KONIG__CLIENTID_HPP

