

#ifndef __H_HUICPP_CRYPTO_SHA_H__
#define __H_HUICPP_CRYPTO_SHA_H__

#include <huicpp.h>

namespace HUICPP {

namespace CRYPTO {

class Sha {
public:
    constexpr Sha() noexcept = default;

    ~ Sha() noexcept = default;

public:
    static constexpr const HN SHA256_BUF_LEN = 32;
    typedef HUCH sha256_buf_t[SHA256_BUF_LEN];    

public:
    static void Sha256(HCSTRR strInData, HMEMR outdata);

    static void Sha256(HCMEMR indata, HMEMR outdata);    

    static HSTR Sha256(HCSTRR instr);

    static HSTR Sha256(HCMEMR instr);

    static HSTR Sha256File(HCSTRR strFileName);

private:
    static void sha256(HCSTRR indata, sha256_buf_t outbuf);

    static void sha256(HCMEMR indata, sha256_buf_t outbuf);

    static void sha256(HCUSZ indata, HSIZE len, sha256_buf_t outbuf);


};

}

}

#endif //__H_HUICPP_CRYPTO_SHA_H__