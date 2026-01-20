/*
 * Created: Joseph Hui Thu Sep 21 2017
 * Description: 
  		Wrapper for huicpp thread once.
*/

#ifndef __H_HUICPP_THREAD_ONCE_H__
#define __H_HUICPP_THREAD_ONCE_H__


#include "hthread.h"

namespace HUICPP {


using thread_once_base = threadtype_base<pthread_once_t>;
class HThreadOnce : public thread_once_base {
public:
    using base = thread_once_base;
	using native_type = typename base::native_type;
	using native_pointer = typename base::native_pointer;
	using const_native_pointer = typename base::const_native_pointer;
	typedef void (*thread_once_fun_t)(void);

public:
    HThreadOnce(); 

    virtual ~HThreadOnce();

public:
    void Once (thread_once_fun_t fun);

};


}

#endif // __H_HUICPP_THREAD_ONCE_H__
