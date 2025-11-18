
#include "logfileroll_detail.h"
#include <sstream>
#include <iomanip>

namespace HUICPP {

namespace detail {

namespace log {

namespace {

struct log_half_hour_controler {
    typedef HN HourType;

    // Variables of hour type.
    static constexpr const HourType ZERO = 0;
    static constexpr const HourType THIRTY = 1;
    static constexpr const HourType END = 2;

    static const char* const LOG_HALF_HOUR_TYPE_NAMES[END]; 

    static HCSZ GetHalfHourName(HourType ht) noexcept {
        return LOG_HALF_HOUR_TYPE_NAMES[ht];
    }

    static HourType GetHalfHourType(HN hour) noexcept { return (hour < 30) ? ZERO : THIRTY; }
};

const char* const log_half_hour_controler::LOG_HALF_HOUR_TYPE_NAMES[END] = {"00", "30"};


struct log_10_mintues_controler {
    typedef HN MintuesType;

    // Variables of mintues type.
    static constexpr const MintuesType ZERO = 0;
    static constexpr const MintuesType TEN = 1;
    static constexpr const MintuesType TWT = 2;
    static constexpr const MintuesType THT = 3;
    static constexpr const MintuesType FOT = 4;
    static constexpr const MintuesType FIT = 5;
    static constexpr const MintuesType END = 6;

    static const char* const LOG_TEN_MINTUES_TYPE_NAMES[END];        

    static HCSZ Get10MintuesName(MintuesType mt) noexcept {
        return LOG_TEN_MINTUES_TYPE_NAMES[mt];
    }

    static MintuesType Get10MintuesType(MintuesType mm) noexcept { return mm / 10; }
};

const char* const log_10_mintues_controler::LOG_TEN_MINTUES_TYPE_NAMES[END] = {"00", "10", "20", "30", "40", "50"};

}

LogfileRollbackStrategy::LogfileRollbackStrategy(HCSTRR strFileName, ROLLOVER_VALUE type) noexcept 
    : m_value(type),
    m_mtTime(),
    m_filename(strFileName) {

}


bool LogfileRollbackStrategy::NeedRollback(const HMicroTime& cmt) const {
    struct tm tm_in, tm_me;
    cmt.GetTimeTm(tm_in);
    m_mtTime.GetTimeTm(tm_me);

    if (tm_in.tm_year != tm_me.tm_year or tm_in.tm_mon != tm_me.tm_mon
        or tm_in.tm_mday != tm_me.tm_mday) {
        return true;
    }

    if (m_value == DAY) {
        return false;
    }

    if (tm_in.tm_hour != tm_me.tm_hour) {
        return true;
    }

    if (m_value == HOUR) {
        return false;
    }

    if (log_half_hour_controler::GetHalfHourType(tm_in.tm_hour) !=
        log_half_hour_controler::GetHalfHourType(tm_me.tm_hour)) {
        return true;
    }

    if (m_value == HALFHOUR) {
        return false;
    }

    if (log_10_mintues_controler::Get10MintuesType(tm_in.tm_min) != 
        log_10_mintues_controler::Get10MintuesType(tm_me.tm_min)) {
        return true;
    }

    if (m_value == TENMIN) {
        return false;
    }
    
    return false;
}


HSTR LogfileRollbackStrategy::Rollback() noexcept {
    m_mtTime.Touch();
    return GetFileName();
}


HSTR LogfileRollbackStrategy::GetFileName(HCSTRR endstr) const noexcept {
    struct tm _tm;
    m_mtTime.GetTimeTm(_tm);
    std::ostringstream ss;
    ss << m_filename << "_";
    ss.fill('0');
    ss  << 1900 + _tm.tm_year 
        << '_' << std::setw(2) << 1 + _tm.tm_mon
        << '_' << std::setw(2) << _tm.tm_mday;

    if (m_value != DAY) {
        ss << '_' << std::setw(2) << _tm.tm_hour;
    }
    
    if (m_value == HALFHOUR) {
        ss << "_" << log_half_hour_controler::GetHalfHourName(
            log_half_hour_controler::GetHalfHourType(_tm.tm_hour));
    } else if (m_value == TENMIN) {
        ss << "_" << log_10_mintues_controler::Get10MintuesName(
            log_10_mintues_controler::Get10MintuesType(_tm.tm_min));
    }
    if (not endstr.empty()) {
        ss << "_" << endstr;
    }
    ss << ".log";
    return ss.str();
}


LogfileRollbackStrategy::ROLLOVER_VALUE LogfileRollbackStrategy::GetRolloverTypeName(HCSTRR strName) {
    for (HN i = 0; i < END; ++i) {
        HSTR str(rollover_names[i]);
        if (str == strName) {
            return i;
        }
    }
    return DAY;
}


const char* const LogfileRollbackStrategy::rollover_names[LogfileRollbackStrategy::END] = {
    "DAY", "HOUR", "HALF-HOUR", "10MIN"
};


}

}

}
