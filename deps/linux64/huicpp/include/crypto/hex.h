#ifndef __HUICPP_CRYPTO_HEX_H__
#define __HUICPP_CRYPTO_HEX_H__

#include "huicpp.h"
#include <array>

namespace HUICPP {

namespace CRYPTO {

class Hex {
public:
    Hex() noexcept = default;

    ~ Hex() noexcept = default;

public:
    static HSTR HexMem(HCMEMR mem);

    static HSTR HexStr(HCSTRR str);

    static HSTR HexStr(HCUSZ ptr, HN len);
};

}

}

#endif //__HUICPP_CRYPTO_HEX_H__