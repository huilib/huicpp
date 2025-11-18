/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp process.
*/


#ifndef __H_HUICPP_PROCESS_H__
#define __H_HUICPP_PROCESS_H__

#include "huicpp.h"

namespace HUICPP {

class HProcess {
public:
	using pid_type = pid_t;
public:



public:
	static bool ProcessExists (const pid_type& pid);

	static void KillProcessAnyway (const pid_type& pid);

	static HRET KillProcess (const pid_type& pid);

	static HRET GetProcName (HSTRR strName);

	static void chlsig_handler(int signo);

private:
	static HRET kill_process(const pid_type& pid, SYS_T signo);

};

}

#endif // __H_HUICPP_PROCESS_H__


