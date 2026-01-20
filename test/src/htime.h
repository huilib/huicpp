/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp string.
*/


#ifndef __H_HUICPP_TIME_H__
#define __H_HUICPP_TIME_H__

#include "huicpp.h"
#include "internal/base_traits.h"
#include "adaptation/ada_time.h"
#include <sys/time.h>

namespace HUICPP{

template<class _Ty> 
struct base_time_traits : public hc_internal::base_traits<_Ty> {

    using base = hc_internal::base_traits<_Ty>;
    using value_type = typename base::value_type;

    using pointer = typename base::pointer;
    using const_pointer = typename base::const_pointer;
    using refference_type = typename base::refference_type;
    using const_refference_type = typename base::const_refference_type;


    using tm_t = struct tm;

    value_type make_me(const tm_t& _tm) {
        return mktime(&_tm);
    }

    static constexpr HN TIME_BUF_LEN = 128;
    typedef HCH time_buf_t[TIME_BUF_LEN];

    static void reset_time_buf(time_buf_t buf) {
        ZERO_MEM(buf, TIME_BUF_LEN);
    }

};

template <class _Ty>
struct time_traits: public base_time_traits<_Ty>{
    using base = base_time_traits<_Ty>;
    using value_type = typename base::value_type;

    using pointer = typename base::pointer;
    using const_pointer = typename base::const_pointer;
    using refference_type = typename base::refference_type;
    using const_refference_type = typename base::const_refference_type;
};

template <>
struct time_traits<time_t>: public base_time_traits<time_t> {

    using base = base_time_traits<time_t>;
    using value_type = base::value_type;

    using pointer = base::pointer;
    using const_pointer = base::const_pointer;
    using refference_type = base::refference_type;
    using const_refference_type = base::const_refference_type;

    static value_type GetDefaultValue() {
        return time(nullptr);
    }
};

template<class _Ty, class _Traits = time_traits<_Ty> >
class timespan {
public:
    using traits_type = _Traits;
    using value_type = typename traits_type::value_type;
    using size_type = typename traits_type::size_type;

    using pointer = typename traits_type::pointer;
    using const_pointer = typename traits_type::const_pointer;
    using refference_type = typename traits_type::refference_type;
    using const_refference_type = typename traits_type::const_refference_type;

    using index_type = typename traits_type::index_type;
    using itype = typename traits_type::itype;
    using ntype = typename traits_type::ntype;


public:
    timespan() 
        : m_span(traits_type::GetInitValue())
    {}

    timespan(const_refference_type span) 
        : m_span(span)
    {}

    timespan(itype lDays, itype nHours = 0, itype nMins = 0, itype nSecs = 0) {
	    m_span = nSecs + 60 * (nMins + 60 * (nHours + 24 * lDays));
    }

    itype GetDays() const {
        return static_cast<itype>(m_span / (24*3600));
    }

    itype GetTotalHours() const {
        return static_cast<itype>(m_span / 3600);
    }

    itype GetHours() const {
	    return GetTotalHours() % 24;
    }

    itype GetTotalMinutes() const {
	    return static_cast<itype>(m_span / 60);
    }

    itype GetMinutes() const {
	    return GetTotalMinutes() % 60;
    }

    itype GetTotalSeconds() const {
	    return static_cast<itype>(m_span);
    }

    itype GetSeconds() const {
	    return static_cast<itype>(m_span % 60);
    }

    const_refference_type GetTimeSpan() const {
	    return m_span;
    }

    timespan operator+(const timespan& span) const {
	    return timespan(m_span + span.m_span);
    }

    timespan operator-(const timespan& span) const {
	    return timespan(m_span - span.m_span);
    }

    timespan& operator+=(const timespan& span) {
	    m_span += span.m_span;
	    return *this;
    }

    timespan& operator-= (const timespan& span) {
        m_span -= span.m_span;
        return *this;
    }

    bool operator==(const timespan& span) const {
	    return m_span == span.m_span;
    }

    bool operator!=(const timespan& span) const {
	    return m_span != span.m_span;
    }

    bool operator<(const timespan& span) const {
	    return m_span < span.m_span;
    }

    bool operator>(const timespan& span) const {
	    return m_span > span.m_span;
    }

    bool operator<=(const timespan& span) const {
	    return m_span <= span.m_span;
    }

    bool operator>=(const timespan& span) const {
	    return m_span >= span.m_span;
    }

public:
    //  * the only valid formats:
    //  %D - # of days
    //  %H - hour in 24 hour format
    //  %M - minute (0-59)
    //  %S - seconds (0-59)
    //  %% - percent sign
    HSTR tostr(HCSZ pszFormat = "%D/%H/%M/%S") const;

private:
    value_type m_span;
};

#include "inl/timespan_inl.h"

using HTimeSpan = HUICPP::timespan<time_t>;

template<class _Ty, class _Traits = time_traits<_Ty> >
class time_basic {
public:
    using traits_type = _Traits;
    using value_type = typename traits_type::value_type;
    using size_type = typename traits_type::size_type;

    using pointer = typename traits_type::pointer;
    using const_pointer = typename traits_type::const_pointer;
    using refference_type = typename traits_type::refference_type;
    using const_refference_type = typename traits_type::const_refference_type;

    using index_type = typename traits_type::index_type;
    using itype = typename traits_type::itype;
    using ntype = typename traits_type::ntype;

    using timespan_t = timespan<_Ty>;
    using tm_t = typename traits_type::tm_t;

    using time_buf_t = typename traits_type::time_buf_t;

    static constexpr decltype(traits_type::TIME_BUF_LEN) TIME_BUF_LEN = traits_type::TIME_BUF_LEN;

public:
    time_basic() : m_time(traits_type::GetInitValue()){}

    time_basic(const_refference_type time) : m_time(time){}

    time_basic(const tm_t& time) {
	    m_time = traits_type::make_me(time);
    }

    time_basic(itype nYear, itype nMonth, itype nDay, 
        itype nHour = 0, itype nMin = 0, itype nSec = 0, itype nDST = -1 ) {

        tm_t atm;
        atm.tm_sec = nSec;
        atm.tm_min = nMin;
        atm.tm_hour = nHour;        
        atm.tm_mday = nDay;        
        atm.tm_mon = nMonth - 1;        // tm_mon is 0 based        
        atm.tm_year = nYear - 1900;     // tm_year is 1900 based
        atm.tm_isdst = nDST;
        m_time = traits_type::make_me(atm);    

    }

    time_basic(const time_basic& time) {
	    m_time = time.m_time;
    }


    void SetAsNow() {
        m_time = traits_type::GetDefaultValue();
    }

    // parse time from string, avaliable format: %[w](y|Y|m|d|H|M|S)
    // %Y: long year field (1970-2026), %y: short year field(00-26)
    // %m: month field (1-12), %d: day field (1-31), 
    // %H: hour filed (0-23),  %M: minute filed (0-59), 
    // %S: second filed (0-59), %%: percent sign
    // w: field width
    // perfect programming art (:
    time_basic& FromStr(HCSTRR szTimeStr, 
        HCSZ szFormat = "%Y-%m-%d %H:%M:%S");

    

    time_basic& operator=(const_refference_type time) {        
        m_time = time;        
        return *this;
    }

    time_basic& operator= (const time_basic& tt) {

        m_time = tt.m_time;
        return *this;

    }

    time_basic& operator+=(const timespan_t& span ) {
        m_time += span.GetTimeSpan();        
        return *this;
    }

    time_basic& operator-=(const timespan_t&  span) {
        m_time -= span.GetTimeSpan();
        return *this;
    }

    timespan_t operator-(const time_basic& time) const {
	    return timespan_t(m_time - time.m_time);
    }

    time_basic operator-(const timespan_t& span) const {
	    return time_basic(m_time - span.GetTimeSpan());
    }

    time_basic operator+(const timespan_t& span) const {
	    return time_basic(m_time + span.GetTimeSpan());
    }

    bool operator==(const time_basic& time) const {
	    return m_time == time.m_time;
    }

    bool operator!=(const time_basic& time) const {
	    return m_time != time.m_time;
    }

    bool operator<(const time_basic& time) const {
	    return m_time < time.m_time;
    }

    bool operator>(const time_basic& time) const {
	    return m_time > time.m_time;
    }

    bool operator<=(const time_basic& time) const {
	    return m_time <= time.m_time;
    }

    bool operator>=(const time_basic& time) const {
	    return m_time >= time.m_time;
    }

	void Touch () {
		m_time = traits_type::GetDefaultValue();
	}

    tm_t* GetGmtTm(tm_t& tm) const;

    tm_t* GetLocalTm(tm_t& tm) const;

    const_refference_type GetTime() const {
	    return m_time;
    }

#define TMT_SEG(mem_seg) tm_t tmt = {0}; \
    HIGNORE_RETURN(GetLocalTm(tmt));    \
    return tmt.mem_seg

    itype GetYear() const {
        TMT_SEG(tm_year + 1900); 
    }

    itype GetMonth() const {
        TMT_SEG(tm_mon + 1);
    }

    itype GetDay() const {
        TMT_SEG(tm_mday);
    }

    itype GetHour() const {
        TMT_SEG(tm_hour); 
    }

    itype GetMinute() const {
        TMT_SEG(tm_min); 
    }

    itype GetSecond() const {
        TMT_SEG(tm_sec);
    }

    itype GetDayOfWeek() const {
        TMT_SEG(tm_wday + 1);
    }
#undef TMT_SEG    

    // formatting using "C" strftime
    HSTR ToString(HCSTRR strFormat = HSTR("%Y-%m-%d %H:%M:%S")) const;

    static time_basic GetCurrentTime();

    // get time now. thread-safe
    static HRET GetNow(time_buf_t buf);

    static HSTR GetNow();

    // get date to buffer. thread-safe
    static HRET GetDate(time_buf_t buf);

    static HSTR GetDate();

    static HSTR GetTimeGroup ();

    static HSTR GetTimeGroup (const time_basic& time);

    static HSTR GetPrevTimeGroup();

private:
    time_t m_time;
};

#include "inl/time_inl.h"

using HTime = HUICPP::time_basic<time_t>;

class HMicroTime {
public:
    explicit HMicroTime(bool bTouch = true) noexcept;

    explicit HMicroTime(HN micsec) noexcept;

    HMicroTime(const HMicroTime& );

    ~ HMicroTime() = default;
    
public:
    HRET SetAsSystemTime() const;

    HLN GetMillSec () const;

    HLN Distance(const HMicroTime&) const;

    void Touch() noexcept;

    const struct timeval& GetTimeVal() const { return m_tv; }

    HSTR GetDateStr() const;

    HSTR GetTimeStr () const;

    void GetTimeTm(struct tm & _tm) const noexcept;

private:
    static HLN timeval_dis(const struct timeval& tv1, const struct timeval& tv2);

private:
    struct timeval m_tv;
};

}

#endif // __H_HUICPP_TIME_H__

