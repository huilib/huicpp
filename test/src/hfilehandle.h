/*
 * Created: Joseph Sep-26-2021
 * Description: 
  		Wrapper for huicpp file handle. 
*/


#ifndef __H_HUICPP_FILE_HANDLE_H__
#define __H_HUICPP_FILE_HANDLE_H__

#include "huicpp.h"
#include "internal/fd_in.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace HUICPP {

class HFileHandle : public hc_internal::iofd_in {
public:
	using base = hc_internal::iofd_in;
	using fd_t = base::fd_t;

public:
	HFileHandle() noexcept;

	explicit HFileHandle(fd_t fd) noexcept;

	explicit HFileHandle(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT,
		mode_t modes = S_IRUSR | S_IWUSR);

	HFileHandle(const HFileHandle&) = delete;

	HFileHandle(HFileHandle&&) noexcept = default;

	virtual ~HFileHandle();

	HFileHandle& operator= (const HFileHandle&) = delete;

	HFileHandle& operator= (HFileHandle&&) = default;

	HFileHandle Dup() const;

public:
	HRET Open(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT,
		mode_t modes = S_IRUSR | S_IWUSR);

	HRET Dup2(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT,
		mode_t modes = S_IRUSR | S_IWUSR);

	// lock and unlock
	HRET Lock() const;

	HRET LockShare() const;

	HRET UnLock() const;

	HOFF WriteString(HCSTRR str) const;

	void ReadString(HSTRR str) const;

	HRET Sync() const noexcept;

	HRET SetToZero() const noexcept;

private:
	HRET dolock(HN op) const;

public:
	static HFileHandle TmpFile();

};

}



#endif // __H_HUICPP_FILE_HANDLE_H__

