

#ifndef __H_HUICPP_DETAIL_LAZYSINGLETON_STATICSINGLETONHOLDER_H__
#define __H_HUICPP_DETAIL_LAZYSINGLETON_STATICSINGLETONHOLDER_H__

#include "../../internal/base_traits.h"
#include "../../internal/thunk.h"
#include "../../huicpp.h"
#include <atomic>
#include <typeinfo>

using namespace HUICPP;

using namespace HUICPP::hc_internal;

namespace HUICPP {

namespace detail {

namespace lazysingle_np {

/*
    StaticSingletonHolder make sure the difine and implementation is separate.
    Because the memory of singleton-holder is in the indestructure memory.
    All the implementation must be in source files.
*/

/*
    This is internal-use-only class is used to create all leaked Mayers Singleton.
    It guarantees that only one instance of every such singleton will ever be created,
    ever when requested from different compilation units linked dynamically.

    Hold the instance of singleton-holder.
*/
class StaticSingletonHolder {
private:
    using Self = StaticSingletonHolder;
    using Key = std::type_info;
    using Make = HPTR();
    using Cache = std::atomic<HPTR>;

    /*
        real class information store in golbal variable. NOT the tag_t,
        tag_t only used to pass the parameter information.
    */
    template <typename Ty, typename Tag>
    struct Src { };

    /*
        Arg will be used for extract the class information from the singleton-holder.
    */
    struct Arg {
        Cache cache {};         // should be first field.
        Key const* key;
        Make* make;
        void** debug;

        template<typename Ty, typename Tag>
        constexpr Arg(tag_t<Ty, Tag>) noexcept
            : key{HTYPE_INFO_OF(Src<Ty, Tag>)},
            make{Thunk::make<Ty>},
            debug{ &Self::debug<Ty, Tag> } { }

    };

    template<typename Ty, typename Tag>
    static void* debug;     // visiable to debug.

public:
    template<bool Noexcept>
    struct ArgCreate: private Arg {
        friend class StaticSingletonHolder;
        
        template<typename Ty, typename Tag>
        constexpr ArgCreate(hc_internal::tag_t<Ty, Tag> t) : Arg{t} {
            static_assert(Noexcept == noexcept(Ty()), "mismiatch noexcept");
        }
    };

public:
    /// Create
    ///
    /// Return a pointer in the golbal if it has already been created, or 
    /// creates it. Caches it in the global arg.
    template<typename Ty, typename Tag>
    HATTRI_VISIBILITY_HIDDEN static Ty& Create() {
        return create<Ty>(global<Ty, Tag>());
    }


private:
    /// create 
    /// 
    /// check the object pointer whether is created? return it if the object has created,
    /// or create it and cache in global arg. 
    template<typename Ty, typename..., bool Noexcept = noexcept(Ty())> 
    HATTRI_VISIBILITY_HIDDEN static Ty& create(ArgCreate<Noexcept>& arg) {
        auto const v = arg.cache.load(std::memory_order_acquire);
        auto const p = HLIKELY(!!v) ? v : create_<noexcept(Ty())>(arg);
        return *static_cast<Ty*>(p);
    }


    template<typename Ty, typename Tag, typename R = ArgCreate<noexcept(Ty())> >
    HALWAYS_INLINE static /*constexpr*/ R& global() {

        static ArgCreate<noexcept(Ty())> _arg{tag<Ty, Tag>};
        return _arg;

    }

private:
    /*
        detail implementation in source, this function will connect with 
        indestructure memory.
    */
    template<bool Noexcept>
    HNON_INLINE static HPTR create_(Arg& arg) noexcept(Noexcept);

};

template <typename T, typename Tag>
void* StaticSingletonHolder::debug;


}

}

}


#endif //__H_HUICPP_DETAIL_LAZYSINGLETON_STATICSINGLETONHOLDER_H__