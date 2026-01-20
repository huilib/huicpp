/*
 * Created: Joseph Hui Thu 11 Nov 2022
 * Description: 
  		Wrapper for huicpp log message.
*/

#ifndef __H_HUICPP_DETAIL_LOGMESSAGE_H__
#define __H_HUICPP_DETAIL_LOGMESSAGE_H__


#include "logpriority_detail.h"
#include "../../htime.h"

namespace HUICPP {

namespace detail {

namespace log {


class LogMessage {
public:
    static constexpr const HN INVALID_LINE_NO = -1;
public:
    LogMessage(HCSTRR message, LogPriority::PriorityValue value = LogPriority::INFO) noexcept;

    LogMessage(HCSTRR message, HCSTRR filename, HN lineno, LogPriority::PriorityValue value = LogPriority::INFO) noexcept;

    ~ LogMessage() noexcept = default;

public:
    HALWAYS_INLINE LogPriority::PriorityValue GetLogPriority() const noexcept { return m_logvalue; }

    HALWAYS_INLINE const HMicroTime& GetMicroTime() const noexcept { return m_time; }

    HALWAYS_INLINE HCSTRR GetMessage() const noexcept { return m_message; }

    HSTR GetLogMessage() const;

private:
    LogPriority::PriorityValue m_logvalue;
    const HMicroTime m_time;
    HCSTR m_message;
    HCSTR m_filename;
    HN m_lineno;
};


}

}

}

#endif // __H_HUICPP_DETAIL_LOGMESSAGE_H__

