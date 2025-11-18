/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp mstr.
*/


#ifndef __H_HUICPP_MSTR_H__
#define __H_HUICPP_MSTR_H__

#include "huicpp.h"

namespace HUICPP{


class HMstr : public HMSTR {
public:
    using base_class = HMSTR;
    using key_type = base_class::key_type;
    using mapped_type = base_class::mapped_type;
    using iterator = base_class::iterator;
    using const_iterator = base_class::const_iterator;
    using allocator_type = base_class::allocator_type;
    using key_compare = base_class::key_compare;
    using value_type = base_class::value_type;
    using size_type = base_class::size_type;

public:
    HMstr () {  }

    HMstr (const HMstr& _right);

    explicit HMstr(const key_compare& _kcp);

    HMstr(const key_compare& kcp, const allocator_type& al);

    HMstr(HMstr&& right);

    HMstr& operator=(const HMstr& other);

public:
    void SetValue (HCSTRR key, HCSTRR val) noexcept;

    void SetValue (HCSTRR key, HN val);

    // if cann't locate the element matched with the key, return a empty-string.
    HCSTRR GetValue (HCSTRR key) const noexcept;

    HCSTRR GetValue (HCSTRR key, HCSTRR default_val) const noexcept;

    // return 0, if the key is not in the map.
    HN GetInt (HCSTRR key) const noexcept;

    HN GetInt (HCSTRR key, HN n) const noexcept;

    bool IsDigit (HCSTRR key) const;

    bool Exists (HCSTRR key) const;

    size_type Remove(HCSTRR key);

    HSTR ToParamString () const;

    void SetValueFormat (HCSTRR key, HCSZ fmt, ...);

    HSTR ToJson () const;


private:
    static constexpr HCSZ AND_SEQ = "&";
    static constexpr HCSZ VAL_SEQ = "=";
};
    
}

#endif //__H_HUICPP_MSTR_H__
