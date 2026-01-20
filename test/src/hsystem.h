/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for system call.
*/

#ifndef __H_HUICPP_SYSTEM_H__
#define __H_HUICPP_SYSTEM_H__

#include "huicpp.h"

namespace HUICPP {

class HSystem {
public:
    HSystem() noexcept = default;

    ~ HSystem() noexcept = default;

public:
    static SYS_RET_T ShellCall (HCSTRR cmd) noexcept;

    static void Reboot() noexcept;

    static void Shutdown() noexcept;
};

}


#endif // __H_HUICPP_SYSTEM_H__

