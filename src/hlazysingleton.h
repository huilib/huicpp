/**********************************************************
 * Created: Joseph Fri 06 2022
 * Description: 
 * 		Wrapper for huicpp lazy singleton.
 **********************************************************/


#ifndef __H_HUICPP_LAZY_SINGLETON_H__
#define __H_HUICPP_LAZY_SINGLETON_H__

#include "huicpp.h"
#include "detail/lazysingleton/lazysingleton.h"


namespace HUICPP{

template<typename _Ty, typename Tag = detail::lazysingle_np::DefaultLazyTag>
class HLazySingleton {
public:
    using value_type = _Ty;
    using pointer_type = _Ty*;

    static pointer_type Get() {
        return get_holder().Get();
    }

private:
    static detail::lazysingle_np::LazySingletonHolder<_Ty>& get_holder() {
        return detail::lazysingle_np::LazySingletonHolder<_Ty>::template Singleton<Tag>();
    }

};


}


#endif // __H_HUICPP_LAZY_SINGLETON_H__


