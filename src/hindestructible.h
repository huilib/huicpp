

#ifndef __H_HUICPP_INDESTRUCTIBLE_H__
#define __H_HUICPP_INDESTRUCTIBLE_H__

#include "huicpp.h"
#include "internal/base_traits.h"

#include <type_traits>
#include <utility>


namespace HUICPP {


/*
*   Indestructible
* 
*   When you need an object that will not get destructed, even at shutdown, and you also
*   want the object stored inline.
* 
*   Use like:
*       void doSomethingWithExpensiveData() {
*           static const Indestructible<map<string, int>> data { 
*               map<string,int> {{"key1", 20}, {"key2", 30}, {"key3", 60}}
*           };
*
*           callSomethingTakingAMapByRef(*data);
*       }
*
*   This should be used only when the instance does not need to be destructed ever.
*
*   This should not be used more generally, e,g., as member fields, etc.
*
*   This is designed as an alternative, but with one fewer allocation at construction
*   time and one fewer pointer dereference at access time.
*/

/*
    used to match the instended tag.
*/
struct factory_constructor_t {
    explicit factory_constructor_t() = default;
};

constexpr factory_constructor_t factory_constructor {};

template<typename Ty>
class Indestructible final {
public:
    template<typename S = Ty, typename = decltype(S())>
    constexpr Indestructible() noexcept(noexcept(Ty()))
        : m_storage { std::in_place } { }

    /*template <typename U = Ty,
      std::enable_if_t<std::is_constructible<Ty, U&&>::value>* = nullptr,
      std::enable_if_t<!std::is_same<Indestructible<Ty>, std::remove_cvref_t<U>>::value>* = nullptr,
      std::enable_if_t<!std::is_convertible<U&&, Ty>::value>* = nullptr>
    explicit constexpr Indestructible(U&& u) noexcept(noexcept(Ty(std::declval<U>())))
      : m_storage{std::in_place, std::forward<U>(u)} {}

    template <typename U = Ty,
      std::enable_if_t<std::is_constructible<Ty, U&&>::value>* = nullptr,
      std::enable_if_t<!std::is_same<Indestructible<Ty>, std::remove_cvref_t<U>>::value>* = nullptr,
      std::enable_if_t<std::is_convertible<U&&, Ty>::value>* = nullptr>
    constexpr Indestructible(U&& u) noexcept(noexcept(Ty(std::declval<U>())))
      : m_storage{std::in_place, std::forward<U>(u)} {}
    
    template <typename... Args, typename = decltype(Ty(std::declval<Args>()...))>
    explicit constexpr Indestructible(Args&&... args) noexcept(noexcept(Ty(std::declval<Args>()...)))
        : m_storage{std::in_place, std::forward<Args>(args)...} {}

    template <typename U, typename... Args,
        typename = decltype(Ty(std::declval<std::initializer_list<U>&>(), std::declval<Args>()...))>
    explicit constexpr Indestructible(std::initializer_list<U> il, Args... args) 
        noexcept(noexcept(Ty(std::declval<std::initializer_list<U>&>(), std::declval<Args>()...)))
        : m_storage{std::in_place, il, std::forward<Args>(args)...} {}
    */
    template <typename Factory>
    constexpr Indestructible(factory_constructor_t, Factory&& factory) noexcept(
        noexcept(factory()))
        : m_storage(factory_constructor, std::forward<Factory>(factory)) {}

    Indestructible(Indestructible const&) = delete;
    Indestructible& operator=(Indestructible const&) = delete;
    
    Ty* get() noexcept { return reinterpret_cast<Ty*>(&m_storage.bytes); }
    Ty const* get() const noexcept {
        return reinterpret_cast<Ty const*>(&m_storage.bytes);
    }
    Ty& operator*() noexcept { return *get(); }
    Ty const& operator*() const noexcept { return *get(); }
    Ty* operator->() noexcept { return get(); }
    Ty const* operator->() const noexcept { return get(); }

    operator Ty&() noexcept { return *get(); }
    operator Ty const&() const noexcept { return *get(); }

private:
    struct Storage {
        hc_internal::aligned_storage_for_t<Ty> bytes;

        /// type Ty's construction parameters...
        template<typename... Args, typename = decltype(Ty(std::declval<Args>()...))>
        explicit constexpr Storage(std::in_place_t, Args&&... args) noexcept(
            noexcept(Ty(std::declval<Args>()...))) {
            ::new (&bytes) Ty(std::forward<Args>(args)...);
        }

        // Create Type Ty with Factory
        template<typename Factory>
        constexpr Storage(factory_constructor_t, Factory factory) noexcept(
            noexcept(factory())) {
            ::new (&bytes) Ty(Factory());
        }
    };

    Storage m_storage {};
};


}


#endif //__H_HUICPP_INDESTRUCTIBLE_H__



