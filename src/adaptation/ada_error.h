/*
* Created: Joseph Hui Tue 24 Aug 2021
* Description: 
    adaptation for error's api.
*/

#ifndef __ADA_ERROR_H__
#define __ADA_ERROR_H__

#include "../huicpp.h"
#include <string.h>

namespace HUICPP {

namespace adapt {

HRET inline hstrerror(HN errnum, HSZ buf, HSIZE buflen) {
    
    HASSERT_RETURN(strerror_r(errnum, buf, buflen) == 0, SYS_FAILED);

    HRETURN_OK;

}


}

}


#endif //__ADA_ERROR_H__

