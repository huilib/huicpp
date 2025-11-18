



#include "happlock.h"
#include <unistd.h>
#include <sys/file.h>


namespace HUICPP {

HAppLock::HAppLock(HCSTRR strFileName) 
    : HFileHandle(strFileName, O_CREAT | O_RDWR, 0644) {

}


HAppLock::~ HAppLock() {

    HIGNORE_RETURN(UnlockApp());

}



HRET HAppLock::LockApp () const {

    HASSERT_RETURN(lockf(Fd(), F_TEST, 0) == 0, INVL_PARA);

    HASSERT_RETURN(lockf(Fd(), F_LOCK, 0) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HAppLock::UnlockApp () const {

    HASSERT_RETURN(lockf(Fd(), F_UNLCK, 0) == 0, SYS_FAILED);

    HRETURN_OK;

}



HProcessLock::HProcessLock(HCSTRR strFileName) 
    : HFileHandle(strFileName, O_CREAT | O_RDWR, 0644) {

    HIGNORE_RETURN(Lock());

}

HProcessLock::~HProcessLock() {

    HIGNORE_RETURN(UnLock());

}



}

