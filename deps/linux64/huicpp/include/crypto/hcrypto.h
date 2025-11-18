

#ifndef __HUICPP_CRYPTO_H__
#define __HUICPP_CRYPTO_H__

#include <huicpp.h>

namespace HUICPP {

namespace CRYPTO {

class Cryptor {
public:
    Cryptor() = default;

    ~ Cryptor() = default;

public:
    HRET Base64UrlEncode (HCSTRR strin, HSTRR strout);

    HRET Base64UrlDecode (HCSTRR strin, HSTRR strout);

    HRET Des3Base64UrlEncode (HCSTRR strin, HSTRR strout, HCSTRR strPassword);

    HRET Des3Base64UrlDecode (HCSTRR strin, HSTRR strout, HCSTRR strPassword);

};

}

}


#endif //__HCRYPTO_H__
