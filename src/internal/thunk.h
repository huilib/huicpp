

#ifndef __H_HUICPP_INTERNAL_THUNK_H__
#define __H_HUICPP_INTERNAL_THUNK_H__

#include "../huicpp.h"

namespace HUICPP {

namespace hc_internal{

//  thunk
//
//  A carefully curated collection of generic general-purpose thunk templates:
//  * make: operator new with given arguments
//  * ruin: operator delete
//  * make_copy: operator new with copy-ctor
//  * make_move: operator new with move-ctor
//  * ctor: in-place constructor with given arguments
//  * dtor: in-place destructor
//  * ctor_copy: in-place copy-constructor
//  * ctor_move: in-place move-constructor
//  * noop: no-op function with the given arguments
//  * fail: terminating function with the given return and arguments
struct Thunk {

    template <typename T, typename... A>
    static void* make(A... a) {
        return new T(static_cast<A>(a)...);
    }
    template <typename T>
    static void ruin(void* const ptr) noexcept {
        delete static_cast<T*>(ptr);
    }
    template <typename T>
    static void* make_copy(void const* const src) {
        return new T(*reinterpret_cast<T const*>(src));
    }
    template <typename T>
    static void* make_move(void* const src) {
        return new T(static_cast<T&&>(*reinterpret_cast<T*>(src)));
    }

    template <typename T, typename... A>
    static void* ctor(void* const ptr, A... a) {
        return ::new (ptr) T(static_cast<A>(a)...);
    }
    template <typename T>
    static void dtor(void* const ptr) noexcept {
        static_cast<T*>(ptr)->~T();
    }
    /*template <typename T>
    static void* ctor_copy(void* const dst, void const* const src) noexcept(
        noexcept(T(FOLLY_DECLVAL(T const&)))) {
        return ::new (dst) T(*reinterpret_cast<T const*>(src));
    }
    template <typename T>
    static void* ctor_move(void* const dst, void* const src) noexcept(
        noexcept(T(FOLLY_DECLVAL(T&&)))) {
        return ::new (dst) T(static_cast<T&&>(*reinterpret_cast<T*>(src)));
    }

    template <std::size_t Size, std::size_t Align>
    static void* operator_new() {
        return folly::operator_new(Size, std::align_val_t(Align));
    }
    template <std::size_t Size, std::size_t Align>
    static void* operator_new_nx() {
        return folly::operator_new(Size, std::align_val_t(Align), std::nothrow);
    }
    template <std::size_t Size, std::size_t Align>
    static void operator_delete(void* const ptr) noexcept {
        return folly::operator_delete(ptr, Size, std::align_val_t(Align));
    }

    template <typename... A>
    static void noop(A...) noexcept {}

    template <typename R, typename... A>
    static R fail(A...) noexcept {
        std::terminate();
    }*/

};

}

}


#endif // __H_HUICPP_INTERNAL_THUNK_H__