

#include "delaytrigger_in.h"


namespace HUICPP {

namespace detail {

namespace scheduler_np {


TriggerSet::TriggerSet() noexcept 
    : m_triggers() {

    for (triggers_t::size_type i = 0; i < m_triggers.size(); ++i) {

        m_triggers[i] = nullptr;

    }

}


trigger_id TriggerSet::CreateTrigger(trigger_proc_t&& new_trigger) noexcept {

    for (triggers_t::size_type i = 0; i < m_triggers.size(); ++i) {

        if (not m_triggers[i]) {

            m_triggers[i].swap(new_trigger);

            return HTO_INT(i);

        }

    }

    return -1;

}


void TriggerSet::RemoveTrigger(trigger_id tid) noexcept {

    if (tid < 0 or tid >= max_trigger_count) {
        return ;
    }

    m_triggers[tid] = nullptr;

}


void TriggerSet::TriggeUpAll() const {

    for (triggers_t::size_type i = 0; i < m_triggers.size(); ++i) {

        if (m_triggers[i]) {

            m_triggers[i]();

        }

    }

}


}

}

}