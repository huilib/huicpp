

#include "selecthandlers_in.h"


namespace HUICPP {

namespace detail {

namespace scheduler_np {

SelectHandlerMap::SelectHandlerMap() noexcept
    : m_handlers() {

}


SelectHandlerMap::~ SelectHandlerMap() noexcept {

}


void SelectHandlerMap::AddHandle(handler_t socketNum, flags_t conditionSet, 
        background_proc_t&& handlerProc) noexcept {
    
    m_handlers.AssignHandler(socketNum, conditionSet, std::move(handlerProc));

}


void SelectHandlerMap::RemoveHandle(handler_t socketNum) noexcept {

    m_handlers.RemoveHandler(socketNum);

}

    
handler_t SelectHandlerMap::SetupHandleSet(fd_set& rfds, fd_set& wfds, fd_set& efds) const noexcept {

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    HandlerIterator iter(m_handlers);
    HandlerDescriptor* dec = nullptr;
    handler_t res = 0;

    while((dec = iter.Next()) != nullptr) {
        
        if ((dec->GetFlags() & HIT_TEST) == 0) {
            continue;
        }

        if (res < dec->GetHandle()) {
            res = dec->GetHandle();
        }

        if (dec->GetFlags() & HIT_READ) {
            FD_SET(dec->GetHandle(), &rfds);
        }

        if (dec->GetFlags() & HIT_WRITE) {
            FD_SET(dec->GetHandle(), &wfds);
        }

        if (dec->GetFlags() & HIT_EXCEP) {
            FD_SET(dec->GetHandle(), &efds);
        }

    }

    return res;

}


void SelectHandlerMap::CallHandlers(const fd_set& rfds, const fd_set& wfds, const fd_set& efds) const {

    HandlerIterator iter(m_handlers);
    HandlerDescriptor* dec = nullptr;

    while((dec = iter.Next()) != nullptr) {

        flags_t result_events = 0;
        if (FD_ISSET(dec->GetHandle(), &rfds)) {
            result_events |= HIT_READ;
        }

        if (FD_ISSET(dec->GetHandle(), &wfds)) {
            result_events |= HIT_WRITE;
        }

        if (FD_ISSET(dec->GetHandle(), &efds)) {
            result_events |= HIT_EXCEP;
        }

        if ((result_events & dec->GetFlags()) != 0) {
            dec->CallHandler(result_events);
        }

    }

}



}

}

}

