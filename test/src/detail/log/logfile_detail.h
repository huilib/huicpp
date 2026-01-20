

/*
 * Created: Joseph Hui Thu 10 Nov 2022
 * Description: 
  		Wrapper for huicpp log file.
*/

#ifndef __H_HUICPP_DETAIL_LOGFILE_H__
#define __H_HUICPP_DETAIL_LOGFILE_H__


#include "../../huicpp.h"
#include "../../hfilehandle.h"
#include "../../hmutex.h"
#include "logfileroll_detail.h"
#include "logmessage_detail.h"

#include <list>

namespace HUICPP {

namespace detail {

namespace log {

class LogFile final : public HFileHandle {
public:
    using base = HFileHandle;

    static constexpr const HN DEFAULT_BACKUP_LOG_COUNT = 7;

public:
    explicit LogFile(HCSTRR strFileName, LogfileRollbackStrategy::ROLLOVER_VALUE type = LogfileRollbackStrategy::DAY) noexcept;

    ~LogFile() noexcept = default;

public:
    HRET Init();

    HHOT_FUNCTION void Write(const LogMessage& msg);

private:
    void rollBack(const HMicroTime& mt);

    void doRollback(const HMicroTime& mt);        

private:
    LogfileRollbackStrategy m_roll_strategy;
    HMutex m_mutex;
    std::list<HSTR> m_backup_logs;
};

}

}

}


#endif //__H_HUICPP_DETAIL_LOGFILE_H__
