

#include "handlerset_in.h"



namespace HUICPP {

namespace detail {

namespace scheduler_np {


////////// HandlerSet (etc.) implementation //////////

HandlerDescriptor::HandlerDescriptor(HandlerDescriptor* nextHandler) noexcept
    : m_hander(invalid_handler_value), m_flags(0), m_bc() {

    // Link this descriptor into a doubly-linked list:
    if (nextHandler == this or nextHandler == nullptr) { // initialization
        m_next = m_prev = this;
        
    } else {
        m_next = nextHandler;
        m_prev = nextHandler->m_prev;
        nextHandler->m_prev = this;
        m_prev->m_next = this;
    }

}

HandlerDescriptor::~HandlerDescriptor() noexcept {

    // Unlink this descriptor from a doubly-linked list:
    m_next->m_prev = m_prev;
    m_prev->m_next = m_next;

}


void HandlerDescriptor::CallHandler(flags_t ff) const {

    if (m_bc) {

        m_bc(ff);

    }

}



HandlerSet::HandlerSet() noexcept
    : m_handlers(nullptr) {
    

}

HandlerSet::~HandlerSet() noexcept {

    // Delete each handler descriptor:
    while (m_handlers.m_next != &m_handlers) {
        delete m_handlers.m_next; // changes fHandlers->fNextHandler
    }

}

void HandlerSet::AssignHandler(handler_t socketNum, flags_t conditionSet, background_proc_t&& handlerProc) noexcept {

    if (socketNum <= 0) {
        return ;
    }

    // First, see if there's already a handler for this socket:
    HandlerDescriptor* handler = lookupHandler(socketNum);
    if (handler == nullptr) { // No existing handler, so create a new descr:
        handler = new HandlerDescriptor(m_handlers.m_next);
        handler->SetHandle(socketNum);
    }

    handler->SetFlags(conditionSet);
    handler->SetProc(std::forward<background_proc_t>(handlerProc));

}

void HandlerSet::RemoveHandler(handler_t socketNum) noexcept {

    HandlerDescriptor* handler = lookupHandler(socketNum);
    if (handler != nullptr) {
        delete handler;
    }

}

void HandlerSet::ChangeHandler(handler_t oldSocketNum, handler_t newSocketNum) noexcept {

    HandlerDescriptor* handler = lookupHandler(oldSocketNum);
    if (handler != nullptr) {
        handler->SetHandle(newSocketNum);
    }

}


HandlerDescriptor* HandlerSet::lookupHandler(handler_t socketNum) noexcept {

    HandlerDescriptor* handler;
    HandlerIterator iter(*this);
    while ((handler = iter.Next()) != nullptr) {
        if (handler->GetHandle() == socketNum) {
            break;
        }
    }
    return handler;

}



HandlerIterator::HandlerIterator(HandlerSet& handlerSet) noexcept
  : m_ourset(handlerSet), m_next_ptr(nullptr) {

    Reset();

}

HandlerIterator::~HandlerIterator() noexcept {
}

void HandlerIterator::Reset() noexcept {

    m_next_ptr = m_ourset.m_handlers.m_next;

}

HandlerDescriptor* HandlerIterator::Next() noexcept {

    HandlerDescriptor* result = m_next_ptr;
    
    if (result == &m_ourset.m_handlers) { // no more
        result = nullptr;
    } else {
        m_next_ptr = m_next_ptr->m_next;
    }

    return result;

}




}


}


}












