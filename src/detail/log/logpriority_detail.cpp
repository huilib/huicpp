

#include "logpriority_detail.h"

namespace HUICPP {

namespace detail {

namespace log {


const char* const LogPriority::LOG_PRIORITY_NAMES[END] = {
    "INFO", "WARN", "ERROR"
};


HCSZ LogPriority::GetLogPriority(PriorityValue val) noexcept {
    return LOG_PRIORITY_NAMES[val];    
}


}

}

}