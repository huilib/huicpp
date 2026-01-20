/*
* Created: Joseph Hui Tue 24 Aug 2021
* Description: 
    adaptation for time's api.
*/

#ifndef __ADA_TIME_H__
#define __ADA_TIME_H__

#include "../huicpp.h"
#include <time.h>

namespace HUICPP {

namespace adapt {

HRET inline hgmtime(const time_t* ttp, struct tm* res) {
    
    HASSERT_RETURN(gmtime_r(ttp, res) != nullptr, SYS_FAILED);

    HRETURN_OK;

}



HRET inline hlocaltime(const time_t* ttp, struct tm* res) {
    
    HASSERT_RETURN(localtime_r(ttp, res) != nullptr, SYS_FAILED);

    HRETURN_OK;

}

}

}


#endif //__ADA_TIME_H__

