

#ifndef __H_HUICPP_DETAIL_SCHEDULER_DELAY_QUEUE_H__
#define __H_HUICPP_DETAIL_SCHEDULER_DELAY_QUEUE_H__

#include "../../huicpp.h"
#include <sys/time.h>
#include <functional>


namespace HUICPP {

namespace detail {

namespace scheduler_np {

using tval_t = HLN;
using tv_t = struct timeval;

using delay_proc_t = std::function<void()>;

static const HLN MILLION = 1000000;

///// A "Timeval" can be either an absolute time, or a time interval /////

class Timeval {
public:
    tval_t Seconds() const noexcept {
        return m_tv.tv_sec;
    }

    tval_t Useconds() const noexcept {
        return m_tv.tv_usec;
    }

    int operator>=(const Timeval& arg2) const noexcept;

    int operator<=(const Timeval& arg2) const noexcept {
        return arg2 >= *this;
    }

    int operator<(const Timeval& arg2) const noexcept {
        return !(*this >= arg2);
    }

    int operator>(const Timeval& arg2) const noexcept {
        return arg2 < *this;
    }

    int operator==(const Timeval& arg2) const noexcept {
        return *this >= arg2 && arg2 >= *this;
    }

    int operator!=(const Timeval& arg2) const noexcept {
        return !(*this == arg2);
    }

    void operator+=(class DelayInterval const& arg2) noexcept;
    void operator-=(class DelayInterval const& arg2) noexcept;
    // returns ZERO iff arg2 >= arg1

protected:
    Timeval(tval_t seconds, tval_t useconds) noexcept
        : m_tv{seconds, useconds} {
    }

    Timeval(const Timeval& other) noexcept 
        : m_tv{other.m_tv.tv_sec, other.m_tv.tv_usec} {
    }

    Timeval& operator=(const Timeval& other) noexcept {
        m_tv.tv_sec = other.m_tv.tv_sec;
        m_tv.tv_usec = other.m_tv.tv_usec;
        return *this;
    }

private:
    tv_t m_tv;
};



class DelayInterval operator-(Timeval const& arg1, Timeval const& arg2) noexcept;
// returns ZERO iff arg2 >= arg1


///// DelayInterval /////

class DelayInterval: public Timeval {
public:
    DelayInterval(tval_t seconds, tval_t useconds) noexcept
        : Timeval(seconds, useconds) {}

    DelayInterval(const DelayInterval& other) noexcept 
        : Timeval(other) { }

    DelayInterval& operator=(const DelayInterval& other) noexcept {
        Timeval::operator=(other);
        return *this;
    }
};


DelayInterval operator*(short arg1, const DelayInterval& arg2) noexcept;


class DelayEventTime: public Timeval {
public:
    DelayEventTime(HLN secondsSinceEpoch = 0,
        HLN usecondsSinceEpoch = 0) noexcept
        // We use the Unix standard epoch: January 1, 1970
        : Timeval(secondsSinceEpoch, usecondsSinceEpoch) {}

    DelayEventTime(const DelayEventTime& other) noexcept
        : Timeval(other) { }

    DelayEventTime& operator=(const DelayEventTime& other) noexcept {
        Timeval::operator=(other);
        return *this;
    }
};

DelayEventTime TimeNow() noexcept;


///// DelayQueueEntry /////

class DelayQueueEntry {
public:
    virtual ~DelayQueueEntry() noexcept;

protected: // abstract base class
    DelayQueueEntry(const DelayInterval& delay) noexcept;

    virtual void handleTimeout() { };

private:
    friend class DelayQueue;
    DelayQueueEntry* m_next;
    DelayQueueEntry* m_prev;
    DelayInterval m_deltaTimeRemaining;
};


///// DelayQueue /////

class DelayQueue: public DelayQueueEntry {
public:
    DelayQueue() noexcept;
    virtual ~DelayQueue() noexcept;

    void AddEntry(DelayQueueEntry* newEntry) noexcept; // returns a token for the entry
    void UpdateEntry(DelayQueueEntry* entry, const DelayInterval& newDelay) noexcept;
    void RemoveEntry(DelayQueueEntry* entry) noexcept; // but doesn't delete it

    const DelayInterval& TimeToNextAlarm() noexcept;
    void HandleAlarm();

private:
    DelayQueueEntry* head() noexcept { return m_next; }
    void synchronize() noexcept; // bring the 'time remaining' fields up-to-date

private:
    DelayEventTime m_lastSyncTime;
};


/// AlterHandler ///
class AlterHandler : public DelayQueueEntry {
private:
    using base_class_t = DelayQueueEntry;

public:
    AlterHandler(const DelayInterval& di, delay_proc_t&& bc) noexcept;

    ~AlterHandler() noexcept;

private:
    virtual void handleTimeout() override;

private:
    delay_proc_t m_bc;

};


}

}

}


#endif //__H_HUICPP_DETAIL_SCHEDULER_DELAY_QUEUE_H__