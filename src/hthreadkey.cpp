

#include "hthreadkey.h"

namespace HUICPP {


HThreadKey::HThreadKey()
    : base () { }

 
 
HThreadKey::~HThreadKey() {

    pthread_key_delete(GetId());

}


HRET HThreadKey::Create(thread_key_destroy_fun_t fun) {

    HASSERT_RETURN(pthread_key_create(GetPointer(), fun) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HThreadKey::Set (HCPTR cptr) {

    HASSERT_RETURN(pthread_setspecific(GetId(), cptr) == 0, SYS_FAILED);

    HRETURN_OK;

}



HPTR HThreadKey::Get () {

    HPTR res = pthread_getspecific(GetId());

    return res;

}




}