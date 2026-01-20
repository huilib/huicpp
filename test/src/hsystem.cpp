

#include "hsystem.h"
#include <cstdlib>
#include <unistd.h>
#include <sys/reboot.h>

namespace HUICPP {

SYS_RET_T HSystem::ShellCall (HCSTRR cmd) noexcept {
    return system(cmd.c_str());
}


void HSystem::Reboot () noexcept {
    (void)ShellCall("reboot");
    reboot(RB_AUTOBOOT);
    while(1) {
        sleep(1);
        reboot(RB_AUTOBOOT);
    }
}


void HSystem::Shutdown () noexcept {
    system("poweroff");
    system("shutdown -h now");
}


}

