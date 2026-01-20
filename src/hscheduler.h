

#ifndef __H_HUICPP_SCHEDULER_H__
#define __H_HUICPP_SCHEDULER_H__

#include "huicpp.h"
#include "detail/scheduler/selecthandlers_in.h"
#include "detail/scheduler/scheduler_in.h"
#include "detail/scheduler/delaytrigger_in.h"

using namespace HUICPP::detail::scheduler_np;

namespace HUICPP {


class HSingleScheduler : public HUICPP::detail::scheduler_np::SchedulerBase {
private:
    using base_class_t = HUICPP::detail::scheduler_np::SchedulerBase;

public:
    HSingleScheduler() noexcept;

    ~HSingleScheduler() noexcept;

public:
    virtual void SetBackgroundHanding(handler_t sfd, flags_t flags, background_proc_t&& bc) noexcept override;

    virtual void RemoveBackgroundHandling(handler_t sfd) noexcept override;

    virtual trigger_id CreateTrigger(trigger_proc_t&& bc) noexcept;

    virtual void RemoveTrigger(trigger_id tid) noexcept;

private:
    virtual void singleStep() override;

private:
    SelectHandlerMap m_handlers;
    TriggerSet m_trigers;

};


}



#endif // __H_HUICPP_SCHEDULER_H__