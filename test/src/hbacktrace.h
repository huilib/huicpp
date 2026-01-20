

#ifndef __H_BACKTRACE_H__
#define __H_BACKTRACE_H__

#include "systemcall/systemcallbase.h"

namespace HUICPP {

using backtrace_baseclass = SystemCallBase;

class HBackTrace : public backtrace_baseclass {
public:
    static constexpr const HN BT_LINES_COUNT = 128;

public:
    HBackTrace();

    ~HBackTrace();

public:
    HBackTrace& SystemCall();

    HSTR ToString() const;

private:
    HVSTR m_lines;
};

}


#endif // __H_BACKTRACE_H__



