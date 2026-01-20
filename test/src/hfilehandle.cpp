

#include "hfilehandle.h"
#include "htodoholder.h"

#include <unistd.h>
#include <sys/file.h>

namespace HUICPP {

HFileHandle::HFileHandle() noexcept
	: base () {

}


HFileHandle::HFileHandle(fd_t fd) noexcept 
	: base (fd) {

}


HFileHandle::HFileHandle(HCSTRR strFileName, SYS_T flags, mode_t modes) 
	: base(open(strFileName.c_str(), flags, modes)) {

}


HFileHandle::~HFileHandle() {

}


HFileHandle HFileHandle::Dup() const {

	if (IsInvalid()) {
		return HFileHandle();
	}

	fd_t fd = dup(Fd());
	if (fd == invalid_fd) {
		return HFileHandle();
	}

	return HFileHandle(fd);
	
}


HRET HFileHandle::Open(HCSTRR strFileName, SYS_T flags, mode_t modes) {
	fd_t _fd = ::open(strFileName.c_str(), flags, modes);
	HASSERT_RETURN(_fd >= 0, SYS_FAILED);
	SetFd(_fd);
	HRETURN_OK;
}


HRET HFileHandle::Dup2(HCSTRR strFileName, SYS_T flags, mode_t modes) {
	fd_t _fd = ::open(strFileName.c_str(), flags, modes);
	HASSERT_RETURN(_fd >= 0, SYS_FAILED);
	HSCOPE_EXIT { close(_fd); };
	HASSERT_RETURN(dup2(_fd, Fd()) >= 0, SYS_FAILED);
	HRETURN_OK;
}


HRET HFileHandle::Lock() const {
	return dolock(LOCK_EX);
}


HRET HFileHandle::LockShare() const {
	return dolock(LOCK_SH);
}


HRET HFileHandle::UnLock() const {
	return dolock(LOCK_UN);
}


HOFF HFileHandle::WriteString(HCSTRR str) const {
	HOFF ret = base::Write(str.c_str(), str.length());
	HIGNORE_RETURN(Sync());
	return ret;
}


void HFileHandle::ReadString(HSTRR str) const {
	str.clear();
	HSTRING_BUF buf = {0};
	HOFF ret = 0;
	while((ret = Read(buf, HSTRING_LEN)) > 0) {
		str.append(buf, ret);
	}
}


HRET HFileHandle::Sync() const noexcept {
	HASSERT_RETURN(::syncfs(base::Fd()), IO_FAILED);
	HRETURN_OK;
}


HRET HFileHandle::SetToZero() const noexcept {
	HASSERT_RETURN(::ftruncate(base::Fd(), 0) == 0, IO_FAILED);
	HRETURN_OK;
}


HRET HFileHandle::dolock(HN op) const {
	HASSERT_RETURN(::flock(Fd(), op) == 0, SYS_FAILED);
	HRETURN_OK;
}


HFileHandle HFileHandle::TmpFile() {
	FILE* pf = tmpfile64();
	HASSERT_THROW(pf != nullptr, SYS_FAILED);

	HSCOPE_EXIT { fclose(pf); };

	fd_t fd = dup(fileno(pf));
	HASSERT_THROW(fd != invalid_fd, SYS_FAILED);

	return HFileHandle(fd);
}




}