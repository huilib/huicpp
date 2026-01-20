/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		A couple utility.
*/

#ifndef __H_HUICPP_UTIL_H__
#define __H_HUICPP_UTIL_H__


#include "huicpp.h"


namespace HUICPP {

    HN HSleep(HN sec);

    HN HSleep(HN sec, HN nMSec);

    HN HSleep(HN sec, HN nMSec, HN nUSec);

    HN HThreadSleep(HN sec, HN nMSec, HN nUSec);

    HSIZE Ceil2Pow2(HSIZE n); 
    
}

#endif
