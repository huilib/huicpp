

#include "scheduler_in.h"

namespace HUICPP {

namespace detail {

namespace scheduler_np {

SchedulerBase::SchedulerBase() noexcept 
    : m_quit(false),
    m_task_queue() {


}


SchedulerBase::~SchedulerBase() noexcept {

}


void SchedulerBase::StopLoop() noexcept {

    m_quit.store(true, std::memory_order_relaxed);

}


void SchedulerBase::ScheduleDelayTask(HULL microseconds, delay_proc_t&& proc) noexcept {

    DelayInterval timeToDelay(static_cast<HLN>(microseconds / MILLION), static_cast<HLN>( microseconds % MILLION));
    AlterHandler* alarmHandler = new AlterHandler(timeToDelay, std::move(proc));

    m_task_queue.AddEntry(alarmHandler);

}


void SchedulerBase::DoEventLoop() {

    while (not m_quit.load(std::memory_order_relaxed)) {

        singleStep();

    }

}


}

}

}

