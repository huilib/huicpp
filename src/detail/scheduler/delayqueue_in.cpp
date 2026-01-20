
#include "delayqueue_in.h"
#include <climits>


namespace HUICPP {

namespace detail {

namespace scheduler_np {

///// Timeval /////

int Timeval::operator>=(const Timeval& arg2) const noexcept {
  
    return Seconds() > arg2.Seconds()
        or (Seconds() == arg2.Seconds() and Useconds() >= arg2.Useconds());

}


void Timeval::operator+=(const DelayInterval& arg2) noexcept {

    m_tv.tv_sec += arg2.m_tv.tv_sec; 
    m_tv.tv_usec += arg2.m_tv.tv_usec;
    if (m_tv.tv_usec >= MILLION) {
        m_tv.tv_usec -= MILLION;
        ++m_tv.tv_sec;
    }

}


void Timeval::operator-=(const DelayInterval& arg2) noexcept {

    m_tv.tv_sec -= arg2.m_tv.tv_sec; 
    m_tv.tv_usec -= arg2.m_tv.tv_usec;

    if (m_tv.tv_usec < 0) {
        m_tv.tv_usec += MILLION;
        --m_tv.tv_sec;
    }
    if (m_tv.tv_sec < 0)
        m_tv.tv_sec = m_tv.tv_usec = 0;

}


static const DelayInterval DELAY_ZERO(0, 0);
static const DelayInterval DELAY_SECOND(1, 0);
static const DelayInterval DELAY_MINUTE = 60 * DELAY_SECOND;
static const DelayInterval DELAY_HOUR = 60 * DELAY_MINUTE;
static const DelayInterval DELAY_DAY = 24 * DELAY_HOUR;
static const DelayInterval HINIT_PRIORITY_HIGHEST ETERNITY(LONG_MAX, MILLION-1);


class DelayInterval operator-(Timeval const& arg1, Timeval const& arg2) noexcept {

    tval_t secs = arg1.Seconds() - arg2.Seconds();
    tval_t usecs = arg1.Useconds() - arg2.Useconds();

    if (usecs < 0) {
        usecs += MILLION;
        --secs;
    }
    if ((int)secs < 0)
        return DELAY_ZERO;
    else
        return DelayInterval(secs, usecs);

}


DelayInterval operator*(short arg1, const DelayInterval& arg2) noexcept {
 
    tval_t result_seconds = arg1 * arg2.Seconds();
    tval_t result_useconds = arg1 * arg2.Useconds();

    tval_t carry = result_useconds / MILLION;
    result_useconds -= carry * MILLION;
    result_seconds += carry;

    return DelayInterval(result_seconds, result_useconds);

}


///// _EventTime /////

DelayEventTime TimeNow() noexcept {

    tv_t tvNow;

    gettimeofday(&tvNow, NULL);

    return DelayEventTime(tvNow.tv_sec, tvNow.tv_usec);

}

const DelayEventTime THE_END_OF_TIME(LONG_MAX);


///// DelayQueueEntry /////

DelayQueueEntry::DelayQueueEntry(const DelayInterval& delay) noexcept
    : m_next(nullptr), m_prev(nullptr),
    m_deltaTimeRemaining(delay) {

    m_next = m_prev = this;

}


DelayQueueEntry::~DelayQueueEntry() noexcept {
}



///// DelayQueue /////

DelayQueue::DelayQueue() noexcept
  : DelayQueueEntry(ETERNITY) {

    m_lastSyncTime = TimeNow();

}

DelayQueue::~DelayQueue() noexcept {

    while (m_next != this) {

        DelayQueueEntry* entryToRemove = m_next;

        RemoveEntry(entryToRemove);

        delete entryToRemove;

    }

}

void DelayQueue::AddEntry(DelayQueueEntry* newEntry)  noexcept {

    synchronize();

    DelayQueueEntry* cur = head();

    while (newEntry->m_deltaTimeRemaining >= cur->m_deltaTimeRemaining) {
        newEntry->m_deltaTimeRemaining -= cur->m_deltaTimeRemaining;
        cur = cur->m_next;
    }

    cur->m_deltaTimeRemaining -= newEntry->m_deltaTimeRemaining;

    // Add "newEntry" to the queue, just before "cur":
    newEntry->m_next = cur;
    newEntry->m_prev = cur->m_prev;
    cur->m_prev = newEntry->m_prev->m_next = newEntry;

}

void DelayQueue::UpdateEntry(DelayQueueEntry* entry, const DelayInterval& newDelay) noexcept {

    if (entry == nullptr) { 
        return;
    }

    RemoveEntry(entry);

    entry->m_deltaTimeRemaining = newDelay;

    AddEntry(entry);

}


void DelayQueue::RemoveEntry(DelayQueueEntry* entry) noexcept {

    if (entry == nullptr || entry->m_next == nullptr) {
        return;
    }

    entry->m_next->m_deltaTimeRemaining += entry->m_deltaTimeRemaining;
    entry->m_prev->m_next = entry->m_next;
    entry->m_next->m_prev = entry->m_prev;
    entry->m_next = entry->m_prev = nullptr;
    // in case we should try to remove it again
}


const DelayInterval& DelayQueue::TimeToNextAlarm() noexcept {

    if (head()->m_deltaTimeRemaining == DELAY_ZERO) {
        return DELAY_ZERO; // a common case
    }

    synchronize();

    return head()->m_deltaTimeRemaining;

}

void DelayQueue::HandleAlarm()  {

    if (head()->m_deltaTimeRemaining != DELAY_ZERO) {
        synchronize();
    }

    /*if (head()->m_deltaTimeRemaining == DELAY_ZERO) {

        // This event is due to be handled:
        DelayQueueEntry* toRemove = head();
        RemoveEntry(toRemove); // do this first, in case handler accesses queue

        toRemove->handleTimeout();

        delete toRemove;

    }*/

    while(head()->m_deltaTimeRemaining == DELAY_ZERO) {

        // This event is due to be handled:
        DelayQueueEntry* toRemove = head();
        
        RemoveEntry(toRemove); // do this first, in case handler accesses queue

        toRemove->handleTimeout();

        delete toRemove;

    }
}


void DelayQueue::synchronize() noexcept {

    // First, figure out how much time has elapsed since the last sync:
    DelayEventTime timeNow = TimeNow();
    if (timeNow < m_lastSyncTime) {
        // The system clock has apparently gone back in time; reset our sync time and return:
        m_lastSyncTime  = timeNow;
        return;
    }
    DelayInterval timeSinceLastSync = timeNow - m_lastSyncTime;
    m_lastSyncTime = timeNow;

    // Then, adjust the delay queue for any entries whose time is up:
    DelayQueueEntry* curEntry = head();
    while (timeSinceLastSync >= curEntry->m_deltaTimeRemaining) {
        timeSinceLastSync -= curEntry->m_deltaTimeRemaining;
        curEntry->m_deltaTimeRemaining = DELAY_ZERO;
        curEntry = curEntry->m_next;
    }
    curEntry->m_deltaTimeRemaining -= timeSinceLastSync;
}


/// AlterHandler ///
AlterHandler::AlterHandler(const DelayInterval& di, delay_proc_t&& bc) noexcept
    : base_class_t(di), m_bc(bc) {

}


AlterHandler::~AlterHandler() noexcept {

}


void AlterHandler::handleTimeout() {

    if (m_bc) {

        m_bc();

    }

}


}

}

}

