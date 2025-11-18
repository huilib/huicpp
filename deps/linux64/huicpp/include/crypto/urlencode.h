/*
 * @Author: Tom Hui
 * @Date: 2019-12-30 10:53:05
 * @Description: 
 */


#ifndef __HUICPP_CRYPTO_URLENCODE_H__
#define __HUICPP_CRYPTO_URLENCODE_H__

#include <huicpp.h>

namespace HUICPP {

namespace CRYPTO {


class UrlEncode {
public:
    UrlEncode() noexcept = default;

    ~ UrlEncode() noexcept = default;

public:
    static HRET Encode (HCSTRR str, HSTRR dst, HCSTRR safes = HSTR());

    static HRET Decode (HCSTRR str, HSTRR dst);

    static bool IsUrlEncode (HCSTRR str);

};

}

}

#endif //__HUICPP_CRYPTO_URLENCODE_H__
