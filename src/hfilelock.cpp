

#include "hfilelock.h"
#include <unistd.h>


namespace HUICPP {


HFileLock::HFileLock(HCSTRR strFileName) 
	: base(strFileName) {

}


HRET HFileLock::Lock() const{

	return fcntl_lock(F_SETLKW, F_WRLCK, 0, SEEK_SET, 1);

}


HRET HFileLock::UnLock () const{

	return fcntl_lock(F_SETLK, F_UNLCK, 0, SEEK_SET, 1);

}


HRET HFileLock::fcntl_lock(HN iCmd, HN iType, HN iOffset, HN iWhere, HN iLen) const{

	struct flock stLock;
	ZERO_MEM(&stLock, sizeof(struct flock));
	stLock.l_type = iType;
	stLock.l_start = iOffset;
	stLock.l_whence = iWhere;
	stLock.l_len = iLen;

	while(fcntl(Fd(), iCmd, &stLock) < 0) {

		HASSERT_THROW(errno == EINTR, SYS_FAILED);

		continue;

	}

	HRETURN_OK;

}


}



