

#include "lazysingleton.h"

namespace HUICPP {

namespace detail {


static_singleton_manager::myself& static_singleton_manager::Instance() {
    static static_singleton_manager _instance;
    return _instance;
}


static_singleton_manager::static_singleton_manager ()
    : m_mutex(), m_map()  {
}



}

}


//#include "lazysingleton_inl.h"

