/*
 * Created: Joseph Hui Thu Sep 21 2017
 * Description: 
  		Wrapper for huicpp thread key.
*/

#ifndef __H_HUICPP_THREAD_KEY_H__
#define __H_HUICPP_THREAD_KEY_H__


#include "hthread.h"


namespace HUICPP {

using thread_key_base = threadtype_base<pthread_key_t>;
class HThreadKey : public thread_key_base {
public:
    using base = thread_key_base;
	using native_type = typename base::native_type;
	using native_pointer = typename base::native_pointer;
	using const_native_pointer = typename base::const_native_pointer;
	typedef void (*thread_key_destroy_fun_t)(void*);
public:
    HThreadKey();

    virtual ~HThreadKey();

    HRET Create(thread_key_destroy_fun_t fun);

    HRET Set (HCPTR cptr);

    HPTR Get ();

};


}

#endif
