/*
 * Created: Joseph Hui Mon Oct 24 2022
 * Description: 
  		Wraps getpid.
        Calls getpid() and return the returned value. 
        The cache is updated in the child after fork.
*/


#ifndef __H_HUICPP_PROCESS_PID_H__
#define __H_HUICPP_PROCESS_PID_H__


#include "../huicpp.h"


namespace HUICPP {

pid_t HGetPid();

}


#endif //__H_HUICPP_PROCESS_PID_H__
