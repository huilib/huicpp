/*
 * Created: Joseph Hui Thu 10 Nov 2022
 * Description: 
  		Wrapper for huicpp log priority.
*/

#ifndef __H_HUICPP_DETAIL_LOGPRIORITY_H__
#define __H_HUICPP_DETAIL_LOGPRIORITY_H__


#include "../../huicpp.h"

namespace HUICPP {

namespace detail {

namespace log {

struct LogPriority {

    typedef HN PriorityValue;

    // Variables of type log prioritys.
    static constexpr const PriorityValue INFO = 0;
    static constexpr const PriorityValue WARNING = 1;
    static constexpr const PriorityValue ERROR = 2;
    static constexpr const PriorityValue END = 3;


    static const char* const LOG_PRIORITY_NAMES[END];

    /*
        Return the string representation of privoded log priority.
        !!Thread-Safe!!.
    */
    static HCSZ GetLogPriority(PriorityValue) noexcept;
};

}

}

}

#endif //__H_HUICPP_DETAIL_LOGPRIORITY_H__

