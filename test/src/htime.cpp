

#include "htime.h"
#include "utils/base_utils.h"

#include <sstream>
#include <iomanip>

namespace HUICPP{

HMicroTime::HMicroTime(bool bTouch) noexcept
    : m_tv() {
    
    ZERO_MEM(&m_tv, sizeof(m_tv));

    if (bTouch) {
        Touch();
    }
    
}


HMicroTime::HMicroTime(const HMicroTime& other)
    : m_tv(other.m_tv) {
}    


HMicroTime::HMicroTime(HN micsec) noexcept 
    : m_tv() {
    m_tv.tv_sec = micsec / 1000;
    m_tv.tv_usec = micsec % 1000 * 1000;
}


HRET HMicroTime::SetAsSystemTime() const {
    HASSERT_RETURN(settimeofday(&m_tv, nullptr) == 0, SYS_FAILED);
    HRETURN_OK;
}


HLN HMicroTime::GetMillSec () const {
    struct timeval tv = {0, 0};
    (void)gettimeofday(&tv, nullptr);
    return timeval_dis(m_tv, tv);
}


HLN HMicroTime::Distance(const HMicroTime& other) const {
    return timeval_dis(m_tv, other.m_tv);
}


void HMicroTime::Touch() noexcept {
    (void)gettimeofday(&m_tv, nullptr);
}


HSTR HMicroTime::GetDateStr() const {
    struct tm _tm;
    (void)localtime_r(&m_tv.tv_sec, &_tm);
    std::ostringstream ss;
    ss.fill('0');
    ss  << 1900 + _tm.tm_year 
        << '-' << std::setw(2) << 1 + _tm.tm_mon
        << '-' << std::setw(2) << _tm.tm_mday;
    return ss.str();
}


HSTR HMicroTime::GetTimeStr () const {
    struct tm _tm;
    (void)localtime_r(&m_tv.tv_sec, &_tm);
    std::ostringstream ss;
    ss.fill('0');
    ss  << 1900 + _tm.tm_year 
        << '-' << std::setw(2) << 1 + _tm.tm_mon
        << '-' << std::setw(2) << _tm.tm_mday
        << ' ' << std::setw(2) << _tm.tm_hour
        << ':' << std::setw(2) << _tm.tm_min
        << ':' << std::setw(2) << _tm.tm_sec
        << ',' << std::setw(3) << (m_tv.tv_usec / 1000);
    return ss.str();
}


void HMicroTime::GetTimeTm(struct tm & _tm) const noexcept {
    ZERO_MEM(&_tm, sizeof(struct tm));
    (void)localtime_r(&m_tv.tv_sec, &_tm);
}


HLN HMicroTime::timeval_dis(const struct timeval& tv1, const struct timeval& tv2) {
    return (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000;
}


}

