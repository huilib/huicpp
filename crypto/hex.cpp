

#include "hex.h"

namespace HUICPP {

namespace CRYPTO {

namespace {
using hexbyte_t = std::array<HCH, 2>;
using hexbyte_map_t = std::array<hexbyte_t, 256>;

hexbyte_map_t create_hexbyte_map() {
    constexpr HCH hexmap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    hexbyte_map_t byte_to_hex;
    for (size_t i = 0; i < byte_to_hex.size(); ++i) {
        byte_to_hex[i][0] = hexmap[i >> 4];
        byte_to_hex[i][1] = hexmap[i & 15];
    }
    return byte_to_hex;
}

static const hexbyte_map_t hexbyte_map = create_hexbyte_map();

}



HSTR Hex::HexMem(HCMEMR mem) {
    std::string rv(mem.size() * 2, 0);
    for (HSTR::size_type i = 0; i < mem.size(); ++i) {
        HSTR::value_type c = mem[i];
        hexbyte_map_t::const_reference cvr = hexbyte_map[HTO_INT(c)];
        rv[i * 2] = cvr[0];
        rv[i * 2 + 1] = cvr[1];
    }
    return rv;
}

HSTR Hex::HexStr(HCSTRR str) {
    std::string rv(str.size() * 2, '\0');
    for (HSTR::size_type i = 0; i < str.size(); ++i) {
        HSTR::value_type c = str[i];
        hexbyte_map_t::const_reference cvr = hexbyte_map[HTO_INT(c)];
        rv[i * 2] = cvr[0];
        rv[i * 2 + 1] = cvr[1];
    }
    return rv;
}

HSTR Hex::HexStr(HCUSZ ptr, HN len) {
    std::string rv(len * 2, '\0');
    for (HN i = 0; i < len; ++i) {
        HUCH c = ptr[i];
        hexbyte_map_t::const_reference cvr = hexbyte_map[HTO_INT(c)];
        rv[i * 2] = cvr[0];
        rv[i * 2 + 1] = cvr[1];
    }
    return rv;
}


}

}