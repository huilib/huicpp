/*
* Created: Joseph Hui Fri 06 May 2022
* Description:
    Wrapper for file descriptor.
*/


#ifndef __H_HUICPP_INTERNAL_FD_IN_H__
#define __H_HUICPP_INTERNAL_FD_IN_H__

#include "base_traits.h"
#include "error_in.h"
#include "commonmacro_in.h"

#include <sys/uio.h>

namespace HUICPP {

namespace hc_internal {

template <class FUN, class... Args>
SYS_T ffun_in_intr(FUN ff, Args... args) {
    SYS_T ret = 0;
    do {
        ret = ff(args...);
    } while(ret == -1 and errno == EINTR);
    return ret;
}

class fd_in {
public:
    using fd_t = SYS_T;
    static constexpr const fd_t invalid_fd = -1;

protected:
    fd_in () noexcept;

    explicit fd_in (fd_t fd) noexcept;

    fd_in(const fd_in&) = delete;

    fd_in(fd_in&&) noexcept;

    virtual ~ fd_in() noexcept;

    fd_in& operator= (const fd_in&) = delete;
    fd_in& operator= (fd_in&&) noexcept;

public:
    bool IsInvalid() const noexcept { return m_fd == invalid_fd;  }

    void Close();

    void SetAsInvalid() { m_fd = invalid_fd; }

public:
    fd_t Fd() const { return m_fd; }

    void SetFd(fd_t fd) { m_fd = fd; }

private:
    fd_t m_fd;
};


class iofd_in : public fd_in {
public:
    using base = fd_in;
    using fd_t = base::fd_t;
    using size_type = HSIZE;
    using pos_type = HSSIZE;

public:
    iofd_in() noexcept;

    explicit iofd_in(fd_t fd) noexcept;

    iofd_in(const iofd_in&) = delete;

    iofd_in(iofd_in&&) noexcept = default;

    virtual ~ iofd_in() noexcept = default;

    iofd_in& operator= (const iofd_in&) = delete;
    iofd_in& operator= (iofd_in&&) noexcept = default;

public:
    virtual HOFF Read(HPTR buf, HSIZE size) const;

    virtual HOFF Write(HCPTR buf, HSIZE size) const;

    virtual HOFF Pread(HPTR buf, HSIZE size, HOFF off) const;

    virtual HOFF Pwrite(HCPTR buf, HSIZE size, HOFF off) const;

    HSIZE WriteV(const struct iovec* iovec, int count) const;

    HSIZE ReadV(const struct iovec* iovec, int count) const;

public:
    HRET SetNonblocked (bool bFlag = true) const;

    bool IsNonblocked () const noexcept(false);

};

}

}


#endif // __H_HUICPP_INTERNAL_FD_IN_H__


