/**********************************************************
 * Created: Joseph Sep-06-2021
 * Description: 
 *      Define some json utils.
 **********************************************************/


#ifndef __H_HUICPP_JSON_UTILS_H__
#define __H_HUICPP_JSON_UTILS_H__

#include "../huicpp.h"
#include <nlohmann/json.hpp>

namespace HUICPP {

namespace json_ns = nlohmann;
using json_t = json_ns::json;

namespace utils {


#ifndef TO_JSON
#define TO_JSON_IMPL(key) do { \
    js[#key] = obj.key;  \
} while(0) 
#define TO_JSON(key) TO_JSON_IMPL(key)
#endif //TO_JSON


#ifndef FROM_SON
#define FROM_SON_IMPL(key)  do { \
    if (js.contains(#key) and (not js[#key].is_null())) {   \
        obj.key = js[#key].get<decltype(obj.key)>();    \
    }   \
} while(0)
#define FROM_SON(key) FROM_SON_IMPL(key)
#endif //FROM_SON


#ifndef CHARARR_TO_JSON
#define CHARARR_TO_JSON_IMPL(key) do { \
    HSTR str_ ## key;   \
    size_t pos = strlen(obj.key);   \
    if (pos > 0) {  \
        str_ ## key.assign(obj.key, obj.key + pos); \
    }   \
    js[#key] = str_ ## key; \
} while(0)
#define CHARARR_TO_JSON(key) CHARARR_TO_JSON_IMPL(key)
#endif //CHARARR_TO_JSON


#ifndef CHARARR_FROM_JSON
#define CHARARR_FROM_JSON_IMPL(key,len) do { \
    if (js.contains(#key) and (not js[#key].is_null())) {   \
        HSTR str_ ## key = js[#key].get<HSTR>();    \
        strncpy(obj.key, str_ ## key.c_str(), str_ ## key.size() < len ? str_ ## key.size() : len); \
    } \
} while(0)
#define CHARARR_FROM_JSON(key) CHARARR_FROM_JSON_IMPL(key)
#endif //CHARARR_FROM_JSON


#ifndef ARR_TO_JSON 
#define ARR_TO_JSON_IMPL(key,len) do {   \
    json_t arr_ ## key = json_t::array();   \
    for (HN i = 0; i < len; ++i) {  \
        arr_ ## key .push_back(obj.key[i]);     \
    }   \
    js[#key] = arr_ ## key; \
} while(0)
#define ARR_TO_JSON(key,len) ARR_TO_JSON_IMPL(key,len)
#endif //ARR_TO_JSON 


#ifndef ARR_FROM_JSON
#define ARR_FROM_JSON_IMPL(key,len) do {   \
    json_t arr_ ## key = js[#key];  \
    auto tt = obj.key[0];   \
    for (HN i = 0; i < len; ++i) {  \
        obj.key[i] = arr_ ## key[i].get<decltype(tt)>();    \
    }   \
    js[#key] = arr_ ## key; \
} while(0)
#define ARR_FROM_JSON(key,len) ARR_FROM_JSON_IMPL(key,len)
#endif //ARR_FROM_JSON 

#ifndef HGET_FROM_JSON 
#define HGET_FROM_JSON_IMPL(js,obj,key,ty)  do { \
    if (js.contains(#key) and (not js[#key].is_null())) {   \
        obj.key = js[#key].get<ty>();   \
    } \
} while(0)
#define HGET_FROM_JSON(js,obj,key,ty) HGET_FROM_JSON_IMPL(js,obj,key,ty)
#endif // HGET_FROM_JSON

}

}


#endif //__H_HUICPP_JSON_UTILS_H__

