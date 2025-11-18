

#ifndef __HUICPP_CRYPTO_BASE64_H__
#define __HUICPP_CRYPTO_BASE64_H__

#include "huicpp.h"

namespace HUICPP {

namespace CRYPTO {

class Base64 {
public:
    Base64() noexcept = default;

    ~ Base64() noexcept = default;

public:
    static HRET Encode(HCSTRR indata, HSTRR outdata);

    static HRET Encode(HCMEMR indata, HSTRR outdata);

    static HRET Decode(HCSTRR indata, HMEMR outdata);

    static HRET Decode(HCMEMR indata, HMEMR outdata);

private:
    static HRET base64_encode(HCPTR indata, HSIZE inlen, HSZ & outdata, HSIZER outlen, bool newline = false);

    static HRET base64_decode(HCPTR indata, HSIZE inlen, HPTR & outdata, HSIZER outlen, bool newline = false);
};

}

}

#endif //__HUICPP_CRYPTO_BASE64_H__


