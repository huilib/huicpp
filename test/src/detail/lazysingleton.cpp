

#include "lazysingleton.h"

namespace HUICPP {

namespace detail {



class static_singleton_manager {
public:
    using myself = static_singleton_manager;

public:
    static myself& Instance() {
        static static_singleton_manager _instance;
        return _instance;
    }

public:
    static_singleton_manager ()
        : m_mutex(), m_map()  {
    }


    template<typename _Ty>
    HPTR Create() {
        auto exists_ptr = get_existing<_Ty>();
        if (exists_ptr == nullptr) {
            auto const ptr = new _Ty();
            CHECK_NEWPOINT(ptr);
            std::lock_guard<std::mutex> lg(m_mutex);
            m_map[typeid(_Ty)] = ptr;
            return ptr;
        }
        return exists_ptr;
    }

private:
    template<typename _Ty>
    HPTR get_existing() {
        std::lock_guard<std::mutex> lg(m_mutex);
        auto const cit = m_map.find(typeid(_Ty));
        return cit == m_map.cend() ? nullptr : cit->second;
    }


private:
    using pointer_map_t = std::map<std::type_index, HPTR>;
    std::mutex m_mutex;
    pointer_map_t m_map;
};    


}

}


#include "lazysingleton_inl.h"

