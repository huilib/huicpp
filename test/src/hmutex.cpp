

#include "hmutex.h"

namespace HUICPP {


HMutex::HMutex () {

    HIGNORE_RETURN(pthread_mutex_init(&m_id, nullptr));

}

HMutex::HMutex (const HMutexAttr& attr) {

	  HIGNORE_RETURN(pthread_mutex_init(&m_id, attr.GetPointer()));

}


HMutex::~HMutex() {

	  HIGNORE_RETURN(pthread_mutex_destroy(&m_id));

}


HRET HMutex::Lock () { 

	  HASSERT_RETURN(pthread_mutex_lock(&m_id) == 0, SYS_FAILED);

	  HRETURN_OK;

}


HRET HMutex::TryLock () {

    HASSERT_RETURN(pthread_mutex_trylock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HMutex::UnLock () {

    HASSERT_RETURN(pthread_mutex_unlock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}


HCond::HCond() {

    HIGNORE_RETURN(pthread_cond_init(&m_id, nullptr));

}


HCond::HCond(const HCondAttr& attr){

    HIGNORE_RETURN(pthread_cond_init(&m_id, attr.GetPointer()));

}


HCond::~HCond() {

    HIGNORE_RETURN(pthread_cond_destroy(&m_id));

}


HRET HCond::Signal () {

    HASSERT_RETURN(pthread_cond_signal(&m_id) == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HCond::Broatcast () {

    HASSERT_RETURN(pthread_cond_broadcast(&m_id) == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HCond::Wait (HMutex& mutex) {

    HASSERT_RETURN(pthread_cond_wait(&m_id, mutex.GetMutexPointer()) == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HConLock::Wait() {

    return HCond::Wait(*this);

}


HWaitSonLock::HWaitSonLock () 
    : HConLock() {

}


void HWaitSonLock::WaitSonRun (HNR i) {

    HMutex_Lock_UnLock holder(*this);

    while (i == 0) {

        HNOTOK_THROW(Wait());

    }

    i = 0;

}


void HWaitSonLock::SonRun (HNR i) {

    HMutex_Lock_UnLock holder(*this);

    i = 1;

    HNOTOK_THROW(Signal());

}


HRwlock::HRwlock() {

    HIGNORE_RETURN(pthread_rwlock_init(&m_id, nullptr));

}


HRwlock::HRwlock (const HRwlockAttr& attr ) {

    HIGNORE_RETURN(pthread_rwlock_init(&m_id, attr.GetPointer()));

}


HRwlock::~HRwlock () {

    HIGNORE_RETURN(pthread_rwlock_destroy(&m_id));

}


HRET HRwlock::RLock() {

    HASSERT_RETURN(pthread_rwlock_rdlock(&m_id) == 0, SYS_FAILED);

	  HRETURN_OK;

}


HRET HRwlock::TryRLock () {

    HASSERT_RETURN(pthread_rwlock_tryrdlock(&m_id) == 0, SYS_FAILED);

	  HRETURN_OK;

}


HRET HRwlock::WLock() {

    HASSERT_RETURN(pthread_rwlock_wrlock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HRwlock::TryWLock () {

    HASSERT_RETURN(pthread_rwlock_trywrlock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HRwlock::UnLock() {

    HASSERT_RETURN(pthread_rwlock_unlock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;
}


HSpinLock::HSpinLock(HN nShared) {

    HIGNORE_RETURN(pthread_spin_init(&m_id, nShared));

}


HSpinLock::~HSpinLock() {

    HIGNORE_RETURN(pthread_spin_destroy(&m_id));

}



HRET HSpinLock::Lock () {

    HASSERT_RETURN(pthread_spin_lock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}



HRET HSpinLock::TryLock () {

    HASSERT_RETURN(pthread_spin_trylock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}



HRET HSpinLock::UnLock ()  {

    HASSERT_RETURN(pthread_spin_unlock(&m_id) == 0, SYS_FAILED);

    HRETURN_OK;

}



}




