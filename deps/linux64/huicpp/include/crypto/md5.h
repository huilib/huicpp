

#ifndef __HUICPP_CRYPTO_MD5_H__
#define __HUICPP_CRYPTO_MD5_H__

#include <huicpp.h>

namespace HUICPP {

namespace CRYPTO {

class Md5 {
public:
    constexpr Md5() noexcept = default;

    ~ Md5() noexcept = default;

public:
    static constexpr const HN MD5_BUF_LEN = 16;
    typedef HUCH md5_buf_t[MD5_BUF_LEN];    

public:
    static void MD5(HCSTRR indata, HMEMR outdata);
    
    static void MD5(HCMEMR indata, HMEMR outdata);    

    static HSTR MD5(HCSTRR instr);

    static HSTR MD5(HCMEMR instr);

    static HSTR Md5File(HCSTRR strFileName);

private:
    static void md5(HCSTRR indata, md5_buf_t outbuf);

    static void md5(HCMEMR indata, md5_buf_t outbuf);

    static void md5(HCUSZ indata, HSIZE len, md5_buf_t outbuf);

};
        


}

}



#endif //__CRYPTO_MD5_H__
