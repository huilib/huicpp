/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp mutex.
*/


#ifndef __H_HUICPP_MUTEX_H__
#define __H_HUICPP_MUTEX_H__

#include "huicpp.h"
#include <pthread.h>
#include "internal/value_initor_in.h"

//#include "hcond.h"

namespace HUICPP {


DEFINE_OTHER_TRAITS(pthread_mutexattr_t,pthread_mutexattr_init,pthread_mutexattr_destroy);
using HMutexAttr = hc_internal::value_initor<pthread_mutexattr_t>;

class HMutex {
public:
	//friend class HCCond;
	using native_type = pthread_mutex_t;

	HMutex () ;

	HMutex (const HMutexAttr& attr) ;

	HMutex (const HMutex&) = delete;

	HMutex (HMutex&& other) noexcept: m_id(other.m_id)  {
		ZERO_MEM(&other.m_id, sizeof(native_type));
	}

	virtual ~HMutex();

public:
	HMutex& operator= (const HMutex&) = delete;

	HRET Lock () ;

	HRET TryLock ();

	HRET UnLock () ;

	native_type* GetMutexPointer() { return &m_id; }

private:
	native_type m_id;

};

HOLDER_GENERATE(HMutex,Lock,UnLock);
HOLDER_GENERATE(HMutex,TryLock,UnLock);
#define HSCOPE_MUTEX(lock) HMutex_Lock_UnLock holder_ ## __COUNTER__ (lock)

DEFINE_OTHER_TRAITS(pthread_condattr_t,pthread_condattr_init,pthread_condattr_destroy);
using HCondAttr = hc_internal::value_initor<pthread_condattr_t>;

class HCond {
public:
	//friend class HCCond;
	using native_type = pthread_cond_t;

	HCond () ;

	HCond (const HCondAttr& attr);

	HCond (const HCond&) = delete;

	HCond (HCond&& other) noexcept: m_id(other.m_id)  {
		ZERO_MEM(&other.m_id, sizeof(native_type));
	}

	virtual ~HCond();

	HRET Signal () ;

	HRET Broatcast () ;

	HRET Wait (HMutex& mutex) ;

private:
	native_type m_id;

};


class HConLock : public HMutex, public HCond {
public:
	HConLock() { }

	virtual ~ HConLock() { }

	HRET Wait();
};


class HWaitSonLock: public HConLock {
public:
	HWaitSonLock ();

    void WaitSonRun (HNR i);

    void SonRun (HNR i);

};


DEFINE_OTHER_TRAITS(pthread_rwlockattr_t,pthread_rwlockattr_init,pthread_rwlockattr_destroy);
using HRwlockAttr = hc_internal::value_initor<pthread_rwlockattr_t>;

class HRwlock {
 public:
	using native_type = pthread_rwlock_t;

	HRwlock ();

	HRwlock (const HRwlockAttr& attr);

	HRwlock (const HRwlock& rwlock ) = delete;

	HRwlock (HRwlock&& rw ) noexcept : m_id(rw.m_id) {
		ZERO_MEM(&rw.m_id, sizeof(native_type));
	}

	virtual ~ HRwlock ();

	HRET RLock ();

	HRET TryRLock ();

	HRET WLock ();

	HRET TryWLock ();

	HRET UnLock ();

 private:
	native_type m_id;
 };

HOLDER_GENERATE(HRwlock,RLock,UnLock);
HOLDER_GENERATE(HRwlock,TryRLock,UnLock);
HOLDER_GENERATE(HRwlock,WLock,UnLock);
HOLDER_GENERATE(HRwlock,TryWLock,UnLock);


class HSpinLock {
public:
	//friend class HCCond;
	using native_type = pthread_spinlock_t;

	explicit HSpinLock (HN nShared) ;

	HSpinLock (const HMutex&) = delete;

	HSpinLock (HSpinLock&& other) noexcept: m_id(other.m_id)  {
		other.m_id = {0};
	}

	virtual ~HSpinLock();

	HRET Lock () ;

	HRET TryLock ();

	HRET UnLock () ;

private:
	native_type m_id;

};

HOLDER_GENERATE(HSpinLock,Lock,UnLock);
HOLDER_GENERATE(HSpinLock,TryLock,UnLock);


}

#endif //__HMUTEX_H__



