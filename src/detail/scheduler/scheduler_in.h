

#ifndef __H_HUICPP_DETAIL_SCHEDULER_H__
#define __H_HUICPP_DETAIL_SCHEDULER_H__

#include "delayqueue_in.h"
#include "handlerset_in.h"

#include <atomic>


namespace HUICPP {

namespace detail {

namespace scheduler_np {

using task_id = HLN;

/*
    We declare an abstract base scheduler class.
    Users can define some thread-safe interfaces in subclasses...
    For some reason, users may want to use epoll to manage the handlers...
*/
class SchedulerBase {
protected:
    // abstract base class
    SchedulerBase() noexcept;

public:
    virtual ~SchedulerBase() noexcept;

public:
    void StopLoop() noexcept;

    /*
        Schedules a task to occur (after a delay) when we next reach a scheduling point.
        It will not delay if `microseconds` < 0.
    */
    virtual void ScheduleDelayTask(HULL microseconds, delay_proc_t&& proc) noexcept;

    /*
        Monitor a file descriptor in the `DoEventLoop`.
    */
    virtual void SetBackgroundHanding(handler_t sfd, flags_t flags, background_proc_t&& bc) noexcept = 0;

    /*
        Remove a file descriptor.
    */
    virtual void RemoveBackgroundHandling(handler_t sfd) noexcept = 0;

    void DoEventLoop();

protected:
    virtual void singleStep() = 0;

private:
    std::atomic<bool> m_quit;

protected:
    DelayQueue m_task_queue;
};


    
}

}

}


#endif //__H_HUICPP_DETAIL_SCHEDULER_H__