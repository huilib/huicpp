

#include "hscheduler.h"



namespace HUICPP {


HSingleScheduler::HSingleScheduler() noexcept
    : base_class_t(), m_handlers(), m_trigers() {

}


HSingleScheduler::~HSingleScheduler() noexcept {

}


void HSingleScheduler::SetBackgroundHanding(handler_t sfd, flags_t flags, background_proc_t&& bc) noexcept {

    m_handlers.AddHandle(sfd, flags, std::forward<background_proc_t>(bc));

}


void HSingleScheduler::RemoveBackgroundHandling(handler_t sfd) noexcept {

    m_handlers.RemoveHandle(sfd);
        
}


trigger_id HSingleScheduler::CreateTrigger(trigger_proc_t&& bc) noexcept {

    return m_trigers.CreateTrigger(std::forward<trigger_proc_t>(bc));

}


void HSingleScheduler::RemoveTrigger(trigger_id tid) noexcept {

    m_trigers.RemoveTrigger(tid);

}


void HSingleScheduler::singleStep() {

    const DelayInterval& timeToDelay = m_task_queue.TimeToNextAlarm();

    struct timeval tv_timeToDelay;
    tv_timeToDelay.tv_sec = timeToDelay.Seconds();
    tv_timeToDelay.tv_usec = timeToDelay.Useconds();

    // Very large "tv_sec" values cause select() to fail.
    // Don't make it any larger than 1 million seconds (11.5 days)
    const HLN MAX_TV_SEC = MILLION;
    if (tv_timeToDelay.tv_sec > MAX_TV_SEC) {
        tv_timeToDelay.tv_sec = MAX_TV_SEC;
    }

    fd_set readSet;
    fd_set writeSet;
    fd_set exceptionSet;

    handler_t mfd = m_handlers.SetupHandleSet(readSet, writeSet, exceptionSet);
    int selectResult = select(mfd + 1, &readSet, &writeSet, &exceptionSet, &tv_timeToDelay);
    
    if (selectResult < 0) {
        printf("select return negative. Abort\n");
        abort();
    }

    if (selectResult > 0) {
        m_handlers.CallHandlers(readSet, writeSet, exceptionSet);
    }

    m_trigers.TriggeUpAll();

    m_task_queue.HandleAlarm();

}


}


