


#include "urlencode.h"
#include <assert.h>

namespace HUICPP {


namespace CRYPTO {


static inline HUCH ToHex(HUCH x) {

    return  x > 9 ? x + 55 : x + 48;

}


static inline HUCH FromHex(HUCH x) {

    HUCH y;

    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;

}


HRET UrlEncode::Encode (HCSTRR str, HSTRR dst, HCSTRR safes) {

    dst.clear();

    size_t length = str.length();

    for (size_t i = 0; i < length; i++) {

        HCH c = str[i];
        HUCH uc = static_cast<HUCH>(c);

        if (isalnum(uc) or
            (c == '-') or (c == '_') or
            (c == '.') or (c == '~') or 
            safes.find(c) != HSTR::npos) {            
            dst.push_back(c);
        } else if (str[i] == ' ') {
            dst.push_back('+');
        } else {
            dst.push_back('%');
            dst.push_back(ToHex(uc >> 4));
            dst.push_back(ToHex(uc % 16));

        }
    }

    HRETURN_OK;
}


HRET UrlEncode::Decode (HCSTRR str, HSTRR dst) {

    dst.clear();

    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        HCH c = str[i];
        if (c == '+') {
            dst.push_back(' ');
        } else if (c == '%') {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            dst.push_back(high*16 + low);
        } else {
            dst.push_back(str[i]);
        }
    }
    HRETURN_OK;
}


bool UrlEncode::IsUrlEncode (HCSTRR str) {

    auto good_char = [] (HCH c) -> bool {
        if (c >= 'A' && c <= 'Z') return true;
        else if (c >= '0' && c <= '9') return true;
        else return false;
        return false;
    };

    HN len = str.length();
    if (str[len - 2] == '%' or str[len - 1] == '%') {
        return false;
    }

    for (size_t i = 0; i < str.length(); i++) {

        if (str[i] == '%') {
            
            if (not good_char(str[i+1]) or not good_char(str[i+2])) {

                return false;

            }

        }

    }

    return true;

}


}


}
