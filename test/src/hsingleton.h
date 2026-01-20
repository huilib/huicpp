/**********************************************************
 * Created: Joseph Feb-07-2018
 * Description: 
 * 		Wrapper for huicpp singleton.
 **********************************************************/

#ifndef __H_HUICPP_SINGLETON_H__
#define __H_HUICPP_SINGLETON_H__


#include "huicpp.h"

namespace HUICPP{


    template<typename T>
    class HSingleton {
    private:
	HSingleton() {}

	struct create_obj {
	    create_obj () {

		HSingleton<T>::Instance();

	    }

	    inline void do_nothing () const {}

	};

	static create_obj _create_obj;

    public:
	typedef T target_type;

	static target_type* Instance () {

	    static target_type target_obj;

	    _create_obj.do_nothing();

        return (&target_obj);

	}

    };

    template<typename T>
    typename HSingleton<T>::create_obj HSingleton<T>::_create_obj;	

}


#endif // matched __H_HUICPP_SINGLETON_H__











