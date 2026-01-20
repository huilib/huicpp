

#include "systemcallbase.h"
#include <sys/time.h>

namespace HUICPP {

SystemCallBase::SystemCallBase()
    : m_sys_ret(-1), m_call_time_second(0), m_call_time_usec(0) {
    
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    m_call_time_second = tv.tv_sec;
    m_call_time_usec = tv.tv_usec;
        
}


SystemCallBase::~ SystemCallBase() {

}


void SystemCallBase::SetSystemCallReturn(SYS_T ret) noexcept {

    struct timeval tv;
    gettimeofday(&tv, nullptr);

    m_call_time_second = tv.tv_sec;
    m_call_time_usec = tv.tv_usec;

    m_sys_ret = ret;

}


}