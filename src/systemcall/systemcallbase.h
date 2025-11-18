

#ifndef __H_SYSTEMCALL_BASE_H__
#define __H_SYSTEMCALL_BASE_H__

#include "../huicpp.h"

namespace HUICPP {


class SystemCallBase {
public:
    SystemCallBase();

    virtual ~ SystemCallBase();

public:
    void SetSystemCallReturn(SYS_T ret) noexcept;    

    SYS_T GetSystemCallReturn () const noexcept {  return m_sys_ret; }

    HN GetCallTimeSecond() const noexcept { return m_call_time_second; }

    HN GetCallTimeUSec() const noexcept { return m_call_time_usec; }

    virtual HSTR ToString() const = 0;

private:
    SYS_T m_sys_ret;
    HN m_call_time_second;
    HN m_call_time_usec;
};

}


#endif //__H_SYSTEMCALL_BASE_H__

