

#ifndef __H_HUICPP_DETAIL_LAZYSINGLETON_SINGLETON_MANAGER_H__
#define __H_HUICPP_DETAIL_LAZYSINGLETON_SINGLETON_MANAGER_H__


#include "staticsingletonholder_in.h"

namespace HUICPP {

namespace detail {

namespace lazysingle_np {

using static_singleton_manager = StaticSingletonHolder;

template<typename Ty, typename Tag>
HATTRI_VISIBILITY_HIDDEN Ty& CreateGlobal() {

    return static_singleton_manager::Create<Ty, Tag>();

}

}

}

}



#endif // __H_HUICPP_DETAIL_LAZYSINGLETON_SINGLETON_MANAGER_H__

