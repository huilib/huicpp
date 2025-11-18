/*
 * Created: Joseph Hui Thu 11 Nov 2022
 * Description: 
  		Wrapper for huicpp log file roll back stategy.
*/

#ifndef __H_HUICPP_DETAIL_LOGFILEROLL_H__
#define __H_HUICPP_DETAIL_LOGFILEROLL_H__


#include "../../huicpp.h"
#include "../../htime.h"

namespace HUICPP {

namespace detail {

namespace log {


class LogfileRollbackStrategy {
public:
    typedef HN ROLLOVER_VALUE;
    static constexpr const ROLLOVER_VALUE DAY = 0;
    static constexpr const ROLLOVER_VALUE HOUR = 1;
    static constexpr const ROLLOVER_VALUE HALFHOUR = 2;
    static constexpr const ROLLOVER_VALUE TENMIN = 3;
    static constexpr const ROLLOVER_VALUE END = 4;

public:
    explicit LogfileRollbackStrategy(HCSTRR strFileName, ROLLOVER_VALUE type = DAY) noexcept;

    ~ LogfileRollbackStrategy() noexcept = default;
    
    bool NeedRollback(const HMicroTime& cmt) const;

    HSTR Rollback() noexcept;

    HSTR GetFileName(HCSTRR endstr = HSTR()) const noexcept;

    ROLLOVER_VALUE GetRollbackValue() const noexcept { return m_value; }

public:
    static ROLLOVER_VALUE GetRolloverTypeName(HCSTRR strName);

private:
    ROLLOVER_VALUE m_value;
    HMicroTime m_mtTime;
    HCSTR m_filename;

private:
    static const char* const rollover_names[END];
    
};

}

}

}


#endif //__H_HUICPP_DETAIL_LOGFILEROLL_H__

