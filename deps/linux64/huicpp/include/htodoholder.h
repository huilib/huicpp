/**********************************************************
 * Created: Joseph Sep-16-2021
 * Description: 
 * 		Wrapper for huicpp scope guard.
 **********************************************************/


#ifndef __H_HUICPP_TODOHOLDER_H__
#define __H_HUICPP_TODOHOLDER_H__

#include "huicpp.h"

namespace HUICPP {

namespace __todo_holder_internal__ {

/**
 * Internal use for the macro SCOPE_EXIT below.
 */ 
enum class scope_guard_on_exit { };

struct scope_gruad_base {
public:
    void dismiss() noexcept { m_dismissed = true; }
    void rehire() noexcept { m_dismissed = false; }
protected:
    scope_gruad_base(bool dismissed = false) noexcept : m_dismissed(dismissed) { } 

    bool m_dismissed;
};

}


template<class doitfun>
struct ScopeGuard : __todo_holder_internal__::scope_gruad_base {
    using base = __todo_holder_internal__::scope_gruad_base;

    explicit ScopeGuard(doitfun ff) : base(false), m_fun(ff) {  }

    ScopeGuard(ScopeGuard&& other) noexcept:
        base(other.m_dismissed),
        m_fun(std::move(other.m_fun)) {
        other.m_dismissed = true;
    }

    ~ ScopeGuard() {
        if (not m_dismissed) {
            m_fun();
        }
    }

    doitfun m_fun;
};

template<typename _FuncType>
ScopeGuard<typename std::decay<_FuncType>::type> operator+ (
    __todo_holder_internal__::scope_guard_on_exit, _FuncType&& fn) {
    return std::move(ScopeGuard<typename std::decay<_FuncType>::type>(
        std::forward<_FuncType>(fn)));
}

#ifndef HSCOPE_EXIT
#define HSCOPE_EXIT \
    auto HANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = \
    __todo_holder_internal__::scope_guard_on_exit() + [&]()
#endif // HSCOPE_EXIT

} // HUICPP

#endif // __H_HUICPP_TODOHOLDER_H__

