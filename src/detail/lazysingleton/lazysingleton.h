

#ifndef __H_HUICPP_DETAIL_LAZYSINGLETON_LAZYSINGLETON_H__
#define __H_HUICPP_DETAIL_LAZYSINGLETON_LAZYSINGLETON_H__

#include "singletonmanager.h"
#include <typeindex>
#include <mutex>
#include <atomic>

using namespace HUICPP;

namespace HUICPP {

namespace detail {

namespace lazysingle_np {

struct DefaultLazyTag {};



/*
    > holder => manager;
*/    


/*
 * TypeDescriptor is the unique handle for a given singleton. 
 * It is a combination of the type and of the optional name, and used as a key in uniorder_maps.
*/
class TypeDescriptor {
public:
    TypeDescriptor(const std::type_info& value_type_info, const std::type_info& tag_value_info)
      : m_value_type_info(value_type_info), m_tag_value_info(tag_value_info) {
    }

    TypeDescriptor(const TypeDescriptor& other)
      : m_value_type_info(other.m_value_type_info), m_tag_value_info(other.m_tag_value_info) {}

    TypeDescriptor& operator=(const TypeDescriptor& other) {
        if (this != &other) {
            m_value_type_info = other.m_value_type_info;
            m_tag_value_info = other.m_tag_value_info;
        }

        return *this;
    }

    bool operator==(const TypeDescriptor& other) const {
        return m_value_type_info == other.m_value_type_info and m_tag_value_info == other.m_tag_value_info;
    }

private:
    std::type_index m_value_type_info;
    std::type_index m_tag_value_info;
};


template<typename _Ty>
class LazySingletonHolder {
public:
    using value_type = _Ty;
    using pointer_type = _Ty*;

public:
    template<typename tag>
    inline static LazySingletonHolder<_Ty>& Singleton();

    pointer_type Get();

public:
    LazySingletonHolder(const TypeDescriptor& td) noexcept;

private:
    void create_instance();

private:
    template<typename tag>
    struct Impl;

private:
    enum class SingletonHolderState {
        Dead,
        Living,
        LivingInChildAfterFork,
    };


public:
    TypeDescriptor m_type_desc;

    // mutex protects the entire entry during construction and destrution;
    std::mutex m_mutex;

    // State of the singleton entry. If state is Living, m_instance can be safely accessed.
    std::atomic<SingletonHolderState> m_state;

    pointer_type m_instance;

private:
    LazySingletonHolder(const LazySingletonHolder&) = delete;
    LazySingletonHolder(LazySingletonHolder&&) = delete;
    LazySingletonHolder& operator=(const LazySingletonHolder&) = delete;
    LazySingletonHolder& operator=(LazySingletonHolder&&) = delete;
};


}

}

}


#include "lazysingleton_inl.h"


#endif // __H_HUICPP_DETAIL_LAZYSINGLETON_LAZYSINGLETON_H__



