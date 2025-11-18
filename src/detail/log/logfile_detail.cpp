

#include "logfile_detail.h"
#include "../../hfilename.h"

namespace HUICPP {

namespace detail {

namespace log {


LogFile::LogFile(HCSTRR strFileName, LogfileRollbackStrategy::ROLLOVER_VALUE type) noexcept
    : 
    base(),
    m_roll_strategy(strFileName, type),
    m_backup_logs() { 
}


HRET LogFile::Init() {

    HSTR strFileName = m_roll_strategy.Rollback();
    return base::Open(strFileName, O_RDWR | O_CREAT | O_APPEND);

}


HHOT_FUNCTION void LogFile::Write(const LogMessage& msg) {

    rollBack(msg.GetMicroTime());

    base::WriteString(msg.GetLogMessage());

}


void LogFile::rollBack(const HMicroTime& mt) {

    if (m_roll_strategy.NeedRollback(mt)) {        
        doRollback(mt);
    }

}


void LogFile::doRollback(const HMicroTime& mt) {
    HSCOPE_MUTEX(m_mutex);
    if (m_roll_strategy.NeedRollback(mt)) {          

        HSTR strSrcFileName = m_roll_strategy.GetFileName();
        HSTR strNewFileName = m_roll_strategy.GetFileName("bak");

        HSTR strFileName = m_roll_strategy.Rollback();
        HIGNORE_RETURN(base::Dup2(strFileName, O_RDWR | O_CREAT | O_APPEND));

        HFileName fn(strSrcFileName);
        HIGNORE_RETURN(fn.Rename(strNewFileName));

        m_backup_logs.push_back(strNewFileName);

        if (HTO_INT(m_backup_logs.size()) > DEFAULT_BACKUP_LOG_COUNT 
            and m_roll_strategy.GetRollbackValue() == LogfileRollbackStrategy::DAY) {
            HCSTRR strOldFileName = m_backup_logs.front();
            HFileName bfn(strOldFileName);
            HIGNORE_RETURN(bfn.Remove());
            m_backup_logs.pop_front();
        }
    }
}


}

}

}

