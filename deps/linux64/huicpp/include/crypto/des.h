


#ifndef __HUICPP_CRYPTO_DES_H__
#define __HUICPP_CRYPTO_DES_H__


#include <huicpp.h>


namespace HUICPP {

namespace CRYPTO {

class Des3 {
public:
    Des3() noexcept = default;

    ~ Des3() noexcept = default;

public:
    static HRET Encrypt (HCSTRR src, HMEMR dst, HCSTRR passwd);

    static HRET Encrypt (HMEMR src, HMEMR dst, HCSTRR passwd);

    static HRET Decrypt (HMEMR src, HMEMR dst, HCSTRR passwd);

private:
    static HRET des3_encrypt (HCUSZ in, HN inlen, HUSZ out, HNR outlen, HCSTRR password);


    static HRET des3_decrypt (HCUSZ in, HN inlen, HUSZ out, HNR outlen, HCSTRR password);

};

}


}


#endif //__HUICPP_CRYPTO_DES_H__
