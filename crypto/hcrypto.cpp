


#include "hcrypto.h"
#include "urlencode.h"
#include "hutil.h"
#include "base64.h"
#include "des.h"

#include <algorithm>

namespace HUICPP {

namespace CRYPTO {


HRET Cryptor::Base64UrlEncode (HCSTRR strin, HSTRR strout) {

    HSTR src64;
    HNOTOK_RETURN(Base64::Encode (strin, src64));

    HNOTOK_RETURN(UrlEncode::Encode (src64, strout));

    HRETURN_OK;
}


HRET Cryptor::Base64UrlDecode (HCSTRR strIn, HSTRR strOut) {

    strOut.clear();
    HSTR url_src;
    HNOTOK_RETURN(UrlEncode::Decode(strIn, url_src));

    HMEM mem;
    HNOTOK_RETURN(Base64::Decode (url_src, mem));
    strOut.resize(mem.size());
    std::copy(mem.cbegin(), mem.cend(), strOut.begin());

    HRETURN_OK;

}


HRET Cryptor::Des3Base64UrlEncode (HCSTRR strin, HSTRR strout, HCSTRR strPassword) {
    
    strout.clear();
    HMEM des3_dst;
    HNOTOK_RETURN(Des3::Encrypt(strin, des3_dst, strPassword));

    HSTR str64;
    HNOTOK_RETURN(Base64::Encode(des3_dst, str64));

    HNOTOK_RETURN(UrlEncode::Encode(str64, strout));

    HRETURN_OK;
}


HRET Cryptor::Des3Base64UrlDecode (HCSTRR strin, HSTRR strout, HCSTRR strPassword) {
    
    strout.clear();
    HSTR str64;
    HNOTOK_RETURN(UrlEncode::Decode(strin, str64));

    HMEM mem;
    HNOTOK_RETURN(Base64::Decode(str64, mem));

    HMEM des3_src;
    HNOTOK_RETURN(Des3::Encrypt(mem, des3_src, strPassword));

    strout.resize(des3_src.size());
    std::copy(des3_src.cbegin(), des3_src.cend(), strout.begin());

    HRETURN_OK;
}


}

}
