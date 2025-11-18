

#include "hthreadonce.h"



namespace HUICPP {


HThreadOnce::HThreadOnce() 
    : base () {  }


HThreadOnce::~HThreadOnce() {

}


void HThreadOnce::Once (thread_once_fun_t fun){

    HIGNORE_RETURN(pthread_once(GetPointer(), fun));

}

}
