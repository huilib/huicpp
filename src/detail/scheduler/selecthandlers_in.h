

#ifndef __H_HUICPP_DETAIL_SCHEDULER_SELECTHANDLERS_H__
#define __H_HUICPP_DETAIL_SCHEDULER_SELECTHANDLERS_H__

#include "handlerset_in.h"
#include <sys/select.h>


namespace HUICPP {

enum HANDLE_IO_TYPE {
    HIT_NONE = 0,
    HIT_READ = 1 << 0,
    HIT_WRITE = 1 << 1,
    HIT_EXCEP = 1 << 2,
    HIT_TEST = HIT_READ | HIT_WRITE | HIT_EXCEP
};

namespace detail {

namespace scheduler_np {

class SelectHandlerMap {
private:
    using handler_map_t = HandlerSet;

public:
    SelectHandlerMap() noexcept;

    ~ SelectHandlerMap() noexcept;

public:
    void AddHandle(handler_t socketNum, flags_t conditionSet, background_proc_t&& handlerProc) noexcept;

    void RemoveHandle(handler_t socketNum) noexcept;

    handler_t SetupHandleSet(fd_set& rfds, fd_set& wfds, fd_set& efds) const noexcept;

    void CallHandlers(const fd_set& rfds, const fd_set& wfds, const fd_set& efds) const;


private:
    mutable handler_map_t m_handlers;
};

}

}

}


#endif //__H_HUICPP_DETAIL_SCHEDULER_SELECTHANDLERS_H__

