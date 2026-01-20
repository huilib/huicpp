/*
* Created: Joseph Hui Tue 24 Aug 2021
* Description:
    Sinple intergral traits declaration.
*/


#ifndef __H_HUICPP_INTERGRAL_TRAITS_IN_H__
#define __H_HUICPP_INTERGRAL_TRAITS_IN_H__

#include "basictypes_in.h"

namespace HUICPP {

namespace hc_internal{

template<typename...>
struct tag_t {};
/*
template<typename... _Ty>
inline constexpr tag_t<_Ty...> tag {};
*/
template <class _Ty>
struct base_traits {

    using value_type = _Ty;
    using size_type = HSIZE;

    using pointer = value_type*;
    using const_pointer = const value_type*;
    using refference_type = value_type&;
    using const_refference_type = const value_type&;

    using index_type = HN;
    using itype = HN;
    using ntype = HUN;

    static constexpr value_type GetInitValue() {
        return 0;
    }

    static constexpr value_type GetDefaultValue() {
        return 0;
    }

};

//////////////// integral constant ////////////////

template<typename _Ty, _Ty _Val> 
struct hintegral_constant {
    static constexpr _Ty value = _Val;
    typedef _Ty value_type;   
    constexpr operator value_type() const { return value; }
};

template<bool _Val>
using hbool_constant = hintegral_constant<bool, _Val>;

using htrue_constant = hbool_constant<true>;
using hfalse_constant = hbool_constant<false>;
//////////////// integral constant ////////////////


/*
    A variable template that is always false bu requires template arguments to be 
    provided(which are then ignored). This is very useful in very specific cases
    where we want type-depent expressions to defer static_assert's.

    A common use-case:
    template<typename _Ty>
    void foo(_Ty val) {
        if constexpr(std::is_integral_v<_Ty>);
        else if ...
        else static_assert(always_false<_Ty>, "Unsupported type");
    }

    template<typename _Ty>
    struct Foo {
        static_assert(always_false, "unsupported type");
    };
    template<int>
    struct Foo<int> {};

    Foo<int> a; // fine
    Foo<char> b; // fails!
*/
//template<typename...>
//inline constexpr bool alway_false = false;

}


template<typename _Tp>
inline const _Tp& hmin(const _Tp& __a, const _Tp& __b) {
      // concept requirements
    __glibcxx_function_requires(_LessThanComparableConcept<_Tp>)
    if (__b < __a) {
        return __b;
    }
    return __a;
}


template<typename _Tp>
inline const _Tp& hmax(const _Tp& __a, const _Tp& __b) {
    // concept requirements
    __glibcxx_function_requires(_LessThanComparableConcept<_Tp>)
    //return  __a < __b ? __b : __a;
    if (__a < __b) {
        return __b;
    }
    return __a;
}

}


#endif //__H_HUICPP_INTERGRAL_TRAITS_IN_H__
