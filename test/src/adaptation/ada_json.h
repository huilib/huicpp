/*
* Created: Joseph Hui Thu 23 Sep 2021
* Description: 
    adaptation for nlohmann json.
*/

#ifndef __H_HUICPP_ADA_JSON_H__
#define __H_HUICPP_ADA_JSON_H__

#include "../huicpp.h"
#include "../hstr.h"
#include <nlohmann/json.hpp>

namespace HUICPP {

namespace adapt {

namespace json_ns = nlohmann;

class jsonconfig_impl {
public:
    using json_t = json_ns::json;

    HRET load_json_file(HCSTRR strFileName);

    HCSTRR get_value(HCSTRR key) const;

    HCSTRR get_value(HCSTRR key, HCSTRR defVal) const noexcept;

    HN get_int_value(HCSTRR key) const;

    HN get_int_value(HCSTRR key, HN defVal) const noexcept;

    template<class _Ty>
    HRET get_as(HCSTRR key, _Ty& val) const {

        HASSERT_RETURN(HStr::IsBeginWith(key, "/"), INVL_PARA);

        // cannot end with '/'
        HASSERT_RETURN(not HStr::IsEndWith(key, "/"), INVL_PARA);

        HSTR str = key.substr(1);

        HVSTR keys;
        HNOTOK_RETURN(HStr(str).Split("/", keys));

        json_t js = m_json;
        for (HVSTR::size_type i = 0; i < keys.size(); ++i) {          
            HCSTRR s = keys[i];

            if (HStr::IsAllDigit(s)) {
                HASSERT_RETURN(js.is_array(), SRC_ERR);
                HN n = HStr(s).ToN();
                if (i == keys.size() - 1) {
                    val = js[n].get<_Ty>();
                } else {
                    js = js[n];
                }
            } else {
                HASSERT_RETURN(js.contains(s), NOT_EXISTS);
                if (i == keys.size() - 1) {
                    val = js[s].get<_Ty>();
                } else {
                    js = js[s];
                }
            }
            
        }

        HRETURN_OK;
    }

    bool exists(HCSTRR key) const;

    void clear();

    void set_value(HCSTRR key, HCSTRR val) noexcept;

private:
    json_t m_json;
};

}

}


#endif // __H_HUICPP_ADA_JSON_H__
