

#include "staticsingletonholder_in.h"
#include <typeindex>
#include <mutex>


namespace HUICPP {

namespace detail {

namespace lazysingle_np {


namespace {



class StaticSingletonHolderImpl {
public:
    using Self = StaticSingletonHolderImpl;
    using Make = HPTR();

public:
    static Self& Instance() {
        // key-pointer: we will implement as an indestructible memory.
        // Indestructible memory must always live in the .cpp file.
        static StaticSingletonHolderImpl _instance;
        return _instance;
    }

public:
    StaticSingletonHolderImpl ()
        : m_mutex(), m_map() {

    }


    template<typename Arg>
    static HPTR GetExisting(Arg& arg) {
        auto const* const entry = Instance().get_existing_entry(*arg.key);
        auto const ptr = entry ? entry->get_existing() : nullptr;

        if (not ptr) {
            arg.Cache.store(ptr, std::memory_order_release);
        }

        return ptr;
    }

    template<typename Arg>
    static HPTR Create(Arg& arg) {
        auto& entry = Instance().create_entry(*arg.key);
        auto const ptr = entry.create(*arg.make, *arg.debug);
        arg.cache.store(ptr, std::memory_order_release);
        return ptr;
    }


private:

    struct Entry {
        std::atomic<HPTR> m_ptr{};
        std::mutex m_mutex;

        HPTR get_existing() const { return m_ptr.load(std::memory_order_acq_rel); }

        HPTR create(Make& make, HPTR& debug) {
            if (auto const v = m_ptr.load(std::memory_order_acq_rel)) {
                return v;
            }

            std::unique_lock<std::mutex> lock(m_mutex);

            if (auto const v = m_ptr.load(std::memory_order_acq_rel)) {
                return v;
            }

            auto const v = make();
            m_ptr.store(v, std::memory_order_release);
            debug = m_ptr;
            return v;
        }
    };

    
    Entry* get_existing_entry(std::type_info const& key) {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto const it = m_map.find(key);
        return it == m_map.end() ? nullptr : &it->second;
    }

    Entry& create_entry(std::type_info const& key) {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_map[key];
    }


private:
    using pointer_map_t = std::map<std::type_index, Entry>;
    std::mutex m_mutex;
    pointer_map_t m_map;

}; 



}

template<bool Noexcept>
HPTR StaticSingletonHolder::create_(Arg& arg) noexcept(Noexcept) {
    
    return StaticSingletonHolderImpl::Create(arg);

} 


// make sure the implementation in source file.
template HPTR StaticSingletonHolder::create_<true>(Arg& arg) noexcept;

template HPTR StaticSingletonHolder::create_<false>(Arg& arg);

}

}

}