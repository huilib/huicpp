

#ifndef __H_HUICPP_DETAIL_SCHEDULER_HDELAYTRIGGER_H__
#define __H_HUICPP_DETAIL_SCHEDULER_HDELAYTRIGGER_H__

#include "../../huicpp.h"
#include <functional>
#include <array>

namespace HUICPP {

namespace detail {

namespace scheduler_np {


using trigger_id = HN;
using trigger_proc_t = std::function<void()>;
static constexpr const HN max_trigger_count = 128;

class TriggerSet {
private:
    using triggers_t = std::array<trigger_proc_t, max_trigger_count>;

public:
    TriggerSet() noexcept;
    virtual ~ TriggerSet() noexcept = default;

public:
    virtual trigger_id CreateTrigger(trigger_proc_t&& new_trigger) noexcept;

    virtual void RemoveTrigger(trigger_id) noexcept;

    void TriggeUpAll() const;

private:
    triggers_t m_triggers;
};


}

}

}

#endif // __H_HUICPP_DETAIL_SCHEDULER_HDELAYTRIGGER_H__