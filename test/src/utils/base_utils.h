/**********************************************************
 * Created: Joseph Sep-06-2021
 * Description: 
 *      Define some basic utils.
 **********************************************************/


#ifndef __H_HUICPP_BASE_UTILS_H__
#define __H_HUICPP_BASE_UTILS_H__

#include "../huicpp.h"

namespace HUICPP {

namespace utils {

inline HN uper_divied(HN n, HN b) {

    return (n / b) + ((n % b) > 0 ? 1 : 0);

}


template<class _Ty>
inline auto remove_bit(_Ty& s, _Ty v) -> decltype(s) {
    return s & ~v;
}


template<class _Ty>
inline auto set_bit(_Ty& s, _Ty v) -> decltype(s) {
    return s & v;
}


template<class _Ty>
inline bool has_bit(_Ty s, _Ty v) {
    return s & v;
}


inline bool has_bit(HN s, HN v) {
    return s & v;
}


template<class _Ty>
inline auto get_bit(_Ty s, _Ty v) -> decltype(s) {
    return s & v;
}

template<class _Ty>
inline const _Ty& HMax(const _Ty& a, const _Ty& b) { 
    if (a < b) { return b; }
    return a;
}

template<class _Ty>
inline const _Ty& HMin(const _Ty& a, const _Ty& b) { 
    if (b < a) { return b; }
    return a;
}



}

}

#endif //__H_HUICPP_BASE_UTILS_H__

