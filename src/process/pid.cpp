

#include "pid.h"
#include "atfork.h"
#include <unistd.h>

namespace HUICPP {


namespace {

class PidCache {
public:
    PidCache() noexcept
        : m_pid(0) { }

    HALWAYS_INLINE pid_t Get() {
        if (HUNLIKELY(not valid())) {
            init();
        }
        return m_pid;
    }

private:
    bool valid() const noexcept { return m_pid != 0; }

    HCOLD_FUNCTION void init() {
        HAtFork::RegisterHandler(
            "pidcache", []{}, []{}, [this] { this->m_pid = getpid(); });
        m_pid = getpid();
    }

private:
    pid_t m_pid;
};


}


static PidCache pid_cache;


pid_t HGetPid() {

    return pid_cache.Get();

}



}

