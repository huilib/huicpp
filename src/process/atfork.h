/*
 * Created: Joseph Hui Mon Oct 24 2022
 * Description: 
  		Wraps pthread_atfork.
*/


#ifndef __H_HUICPP_PROCESS_ATFORK_H__
#define __H_HUICPP_PROCESS_ATFORK_H__


#include "../huicpp.h"
#include <functional>

namespace HUICPP {

struct HAtFork {
public:
    static void RegisterHandler (HCSTRR strName, std::function<void()> prepare, 
        std::function<void()> parent, std::function<void()> son);

    static void UnregisterHandler(HCSTRR strName);

};

}


#endif //__H_HUICPP_PROCESS_ATFORK_H__
