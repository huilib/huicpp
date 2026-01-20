/**********************************************************
 * Created: Joseph Sep-07-2021
 * Description: 
 * 		Wrapper for huicpp value initor.
 **********************************************************/
#ifndef __INTERNAL_HUICPP_VALUE_INITOR_IN_H__
#define __INTERNAL_HUICPP_VALUE_INITOR_IN_H__

#include "../huicpp.h"
#include <functional>

namespace HUICPP {

namespace hc_internal {

template<class _Ty>
struct value_types {
	using value_type = _Ty;
	using pointer = value_type*;
    using const_pointer = const value_type*;

    static void default_value_func (pointer) {  }
};


template<class _Ty,  class _Types = value_types<_Ty> >
struct value_traits {
    using types = _Types;
    using value_type = typename types::value_type;
	using pointer = typename types::pointer;
    using const_pointer = typename types::const_pointer;

    static void value_init(pointer ptr) {
        types::default_value_func(ptr);
    }

    static void value_uninit(pointer ptr) {
        types::default_value_func(ptr);
    }
    
};


template<class _Ty, 
    class _Traits = value_traits<_Ty> >
class value_initor {
public:
	using traits_type = _Traits;
    using value_type = typename traits_type::value_type;
	using pointer = typename traits_type::pointer;
    using const_pointer = typename traits_type::const_pointer;

    value_initor() 
        : m_value{0} {
        traits_type::value_init(&m_value);   
    }

    value_initor(const value_initor&) = delete;

    value_initor (value_initor&& vi) = delete;

    virtual ~value_initor() {
        traits_type::value_uninit(&m_value);   
    }

    const_pointer GetPointer () const { return &m_value; }

    pointer GetPointer () { return &m_value; }

protected:
    value_type m_value;
};


}

#define DEFINE_OTHER_TRAITS(ty,initor,uninitor) \
namespace hc_internal { \
template<>  \
class value_traits<ty> {    \
public:     \
    using types = value_types<ty>;   \
    using value_type = types::value_type;   \
	using pointer = types::pointer;     \
    using const_pointer = types::const_pointer; \
    static void value_init(pointer ptr) {   \
        initor(ptr);    \
    }   \
    static void value_uninit(pointer ptr) { \
        uninitor(ptr);  \
    }   \
};  \
}



}


#endif //__INTERNAL_HUICPP_VALUE_INITOR_IN_H__
