/*
 * Created: Joseph Sep-26-2021
 * Description: 
  		Wrapper for huicpp application lock. 
*/

#ifndef __H_HUICPP_APP_LOCK_H__
#define __H_HUICPP_APP_LOCK_H__

#include "hfilehandle.h"

namespace HUICPP {


class HAppLock: public HFileHandle {
public:
    using base = HFileHandle;

public:	
	explicit HAppLock (HCSTRR strFileName = HSTR(".lock"));

    ~ HAppLock();

public:
    HRET LockApp () const;

    HRET UnlockApp () const;	

};


class HProcessLock : public HFileHandle {
public:
    explicit HProcessLock (HCSTRR strFileName);

    ~HProcessLock ();


};

}

#endif //__H_HUICPP_APP_LOCK_H__




