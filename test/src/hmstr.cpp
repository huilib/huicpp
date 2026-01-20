


#include "hmstr.h"
#include "hstr.h"
#include <sstream>
#include <cstdarg>

namespace HUICPP{


HMstr::HMstr (const HMstr& _right) 
    : base_class (_right) {

}


HMstr::HMstr(const key_compare& _kcp) 
    : base_class(_kcp) {

}


HMstr::HMstr(const key_compare& kcp, const allocator_type& al) 
    : base_class (kcp, al) { }


HMstr::HMstr(HMstr&& right) 
    : base_class(std::move(right)) { }


HMstr& HMstr::operator=(const HMstr& other)  {

    base_class::operator=(other);
    return *this;

}


void HMstr::SetValue (HCSTRR key, HCSTRR val) noexcept {

    iterator fit = find(key);

    if (fit != end()) {

        fit->second = val;

    } else {

        insert(value_type(key, val));

    }

}


void HMstr::SetValue (HCSTRR key, HN val) {

    SetValue(key, HStr::FromN(val));

}


HCSTRR HMstr::GetValue (HCSTRR key) const noexcept {
    
    static HCSTR s_str;
    return GetValue(key, s_str);

}


HCSTRR HMstr::GetValue (HCSTRR key, HCSTRR default_val) const noexcept {

    const_iterator cfit = find(key);
    if (cfit == cend()) {
        return default_val;
    }

    return cfit->second;

}


HN HMstr::GetInt (HCSTRR key) const noexcept {

    return GetInt(key, 0);

}


HN HMstr::GetInt (HCSTRR key, HN n) const noexcept{

    const_iterator cfit = find(key);
    if (cfit == cend()) {
        return n;
    }

    return HStr(cfit->second).ToN();

}


bool HMstr::IsDigit (HCSTRR key) const {

    return HStr::IsAllDigit(GetValue(key));

}


bool HMstr::Exists (HCSTRR key) const {

    return find(key) != cend();

}


HMstr::size_type HMstr::Remove(HCSTRR key) {

    return erase(key);

}


HSTR HMstr::ToParamString() const {

    std::stringstream ss;

    for (const_iterator it = begin(); it != end(); ++it) {

        ss << it->first << "=" << it->second << "&";

    }

    HSTR res = ss.str();

    res = res.substr(0, res.length() - 1);

    return res;

}


void HMstr::SetValueFormat(HCSTRR key, HCSZ fmt, ...) {

    va_list ap;

    va_start(ap, fmt);

    SetValue(key, HStr::vFormat(fmt, ap));

    va_end(ap);

}


HSTR HMstr::ToJson () const {

    std::stringstream ss;
    ss << "{ ";

    for (const_iterator cit = cbegin(); cit != cend(); ++cit) {
        ss << "{\"" << cit->first << "\":\"" << cit->second << "\"},";
    }

    HSTR res = ss.str();
    HStr::RemoveRight(res, 1);
    res += "}";

    return res;
}


}
