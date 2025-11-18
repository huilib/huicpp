/*
* Created: Joseph Hui Thu 23 Sep 2021
* Description: 
    adaptation for operator system.
*/


#ifndef __H_HUICPP_ADA_SYS_H__
#define __H_HUICPP_ADA_SYS_H__

#include "../huicpp.h"

namespace HUICPP {

namespace adapt {

    inline HCSZ line_br() {

        #ifdef HWIN 
        static HCSZ os_br = "\n\r";
        #else 
        static HCSZ os_br = "\n";
        #endif 
        return os_br;

    }

}

}

#endif // __H_HUICPP_ADA_SYS_H__

