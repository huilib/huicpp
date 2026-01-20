

#ifndef __H_HUICPP_DETAIL_SCHEDULER_HANDLESET_H__
#define __H_HUICPP_DETAIL_SCHEDULER_HANDLESET_H__

#include "../../huicpp.h"
#include "../../internal/fd_in.h"

#include <functional>


namespace HUICPP {

namespace detail {

namespace scheduler_np {


using handler_t = HUICPP::hc_internal::fd_in::fd_t;

static constexpr const handler_t invalid_handler_value = HUICPP::hc_internal::fd_in::invalid_fd;

using flags_t = HUN;
using background_proc_t = std::function<void(flags_t)>;

////////// HandlerSet (etc.) definition //////////

class HandlerDescriptor {
    HandlerDescriptor(HandlerDescriptor* nextHandler) noexcept;
    virtual ~HandlerDescriptor() noexcept;

public:
    handler_t GetHandle() const noexcept { return m_hander; }
    void SetHandle(handler_t h) noexcept { m_hander = h; }
    flags_t GetFlags() const noexcept { return m_flags; }
    void SetFlags(flags_t f) noexcept { m_flags = f; }
    const background_proc_t& GetProc() const noexcept { return m_bc; }
    void SetProc(background_proc_t&& bc)noexcept { m_bc.swap(bc); }
    void CallHandler(flags_t) const;

private:
    handler_t m_hander;
    flags_t m_flags;
    background_proc_t m_bc;

private:
    // Descriptors are linked together in a doubly-linked list:
    friend class HandlerSet;
    friend class HandlerIterator;
    HandlerDescriptor* m_next;
    HandlerDescriptor* m_prev;
};


class HandlerSet {
public:
    HandlerSet() noexcept;
    virtual ~HandlerSet() noexcept;

    void AssignHandler(handler_t socketNum, flags_t conditionSet, background_proc_t&& handlerProc) noexcept;
    void RemoveHandler(handler_t socketNum) noexcept;
    void ChangeHandler(handler_t oldSocketNum, handler_t newSocketNum) noexcept;

private:
    HandlerDescriptor* lookupHandler(handler_t socketNum) noexcept;

private:
    friend class HandlerIterator;
    HandlerDescriptor m_handlers;
};


class HandlerIterator {
public:
    HandlerIterator(HandlerSet& handlerSet) noexcept;
    virtual ~HandlerIterator() noexcept;

    HandlerDescriptor* Next() noexcept; // returns NULL if none
    void Reset() noexcept;

private:
    HandlerSet& m_ourset;
    HandlerDescriptor* m_next_ptr;

};


}

}

}



#endif //__H_HUICPP_DETAIL_SCHEDULER_HANDLESET_H__