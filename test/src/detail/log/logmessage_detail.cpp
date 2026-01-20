
#include "logmessage_detail.h"
#include "../../hthread.h"
#include "../../process/pid.h"
#include <sstream>

namespace HUICPP {

namespace detail {

namespace log {


LogMessage::LogMessage(HCSTRR message, LogPriority::PriorityValue value) noexcept
    : m_logvalue(value),
    m_time(), 
    m_message(message),
    m_filename(),
    m_lineno(INVALID_LINE_NO) {

}


LogMessage::LogMessage(HCSTRR message, HCSTRR filename, HN lineno, LogPriority::PriorityValue value) noexcept 
    : m_logvalue(value), 
    m_time(),
    m_message(message),
    m_filename(filename),
    m_lineno(lineno){

}


HSTR LogMessage::GetLogMessage() const {
    std::ostringstream oss;
    oss << "[" << m_time.GetTimeStr() 
        << "][" << LogPriority::GetLogPriority(m_logvalue) 
        << "][" << HGetPid()
        << "][" << HThread::CurrentThreadHexIdStr() << "]"
        << std::oct 
        << m_message;
    if (not m_filename.empty()) {
        oss << "[" << m_filename << ":" << m_lineno << "]";
    }
    oss << '\n';
    return oss.str();
}



}

}

}