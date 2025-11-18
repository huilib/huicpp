/*
 * Created: Joseph Sep-26-2021
 * Description: 
  		Wrapper for huicpp file lock. 
*/

#ifndef __H_HUICPP_FILE_LOCK_H__
#define __H_HUICPP_FILE_LOCK_H__

#include "hfilehandle.h"

namespace HUICPP {


class HFileLock : public HFileHandle {
public:
	using base = HFileHandle;

public:
	explicit HFileLock(HCSTRR strFileName);

public:
	HRET Lock() const;

	HRET UnLock () const;

private:
	HRET fcntl_lock(HN iCmd, HN iType, HN iOffset, HN iWhere, HN iLen) const;

};



}

#endif //__H_HUICPP_FILE_LOCK_H__









