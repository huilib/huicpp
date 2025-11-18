

#include "fd_in.h"
#include "../huicpp.h"
#include <unistd.h>
#include <sys/fcntl.h>


namespace HUICPP {

namespace hc_internal {


fd_in::fd_in() noexcept 
    : m_fd(invalid_fd) {

}


fd_in::fd_in (fd_t fd) noexcept 
    : m_fd(fd) {

}


fd_in::fd_in(fd_in&& other) noexcept 
    : m_fd(other.m_fd) {
    other.SetAsInvalid();
}


fd_in::~ fd_in() noexcept {
    if (m_fd != invalid_fd) {
        ::close(m_fd);
    }
}


fd_in& fd_in::operator= (fd_in&& other) noexcept {
    m_fd = other.m_fd;
    other.SetAsInvalid();
    return *this;
}


void fd_in::Close() {
    if (not IsInvalid()) {
        close(m_fd);
        SetAsInvalid();
    }
}


iofd_in::iofd_in() noexcept
    : base() {

}


iofd_in::iofd_in(fd_t fd) noexcept 
    : base(fd) {  }


#define CHECK_FD do { if (IsInvalid()) { return -1; } } while(0)

HOFF iofd_in::Read(HPTR buf, HSIZE size) const {
    CHECK_FD;
    return read(Fd(), buf, size);
}


HOFF iofd_in::Write(HCPTR buf, HSIZE size) const {
    CHECK_FD;
    return write(Fd(), buf, size);
}


HOFF iofd_in::Pread(HPTR buf, HSIZE size, HOFF off) const {
    CHECK_FD;
    return pread(Fd(), buf, size, off);
}


HOFF iofd_in::Pwrite(HCPTR buf, HSIZE size, HOFF off) const {
    CHECK_FD;
    return pwrite(Fd(), buf, size, off);
}


HSIZE iofd_in::WriteV(const struct iovec* iovec, int count) const {

    CHECK_FD;

    return ::writev(Fd(), iovec, count);


}


HSIZE iofd_in::ReadV(const struct iovec* iovec, int count) const {

    CHECK_FD;

    return ::readv(Fd(), iovec, count);

}


HRET iofd_in::SetNonblocked (bool bFlag) const{

	HASSERT_RETURN(not IsInvalid(), SRC_ERR);

	SYS_T flags = 0;
	HASSERT_RETURN((flags = fcntl(Fd(), F_GETFL, 0)) >= 0, SYS_FAILED);

	if (bFlag and flags & O_NONBLOCK) {
		HRETURN_OK;
	}

	if (not bFlag and not (flags & O_NONBLOCK)) {
		HRETURN_OK;
	}

	if (bFlag) {
		// set to non-block mode
		HASSERT_RETURN(fcntl(Fd(), F_SETFL, flags | O_NONBLOCK) >= 0, SYS_FAILED);
	} else {
		HASSERT_RETURN(fcntl(Fd(), F_SETFL, flags | ~O_NONBLOCK) > 0, SYS_FAILED);
	}

	HRETURN_OK;
}


bool iofd_in::IsNonblocked () const noexcept(false) {

	SYS_T flags = 0;
	HASSERT_THROW((flags = fcntl(Fd(), F_GETFL, 0)) >= 0, SYS_FAILED);

	return flags & O_NONBLOCK;
}


#undef CHECK_FD

}

}