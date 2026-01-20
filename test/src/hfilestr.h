/*
 * Created: Joseph Aug-29-2021
 * Description: 
  		Wrapper for huicpp file string object. 
*/


#ifndef __H_HUICPP_FILE_STRING_H__
#define __H_HUICPP_FILE_STRING_H__

#include "huicpp.h"
#include "hfilehandle.h"

namespace HUICPP {

class HFileStr final : public HFileHandle {
public:
    using base_t = HFileHandle;

public:
    HFileStr();

    HFileStr(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT);

    HFileStr(const HFileStr&) = delete;

    HFileStr(HFileStr&&) = delete;

    ~ HFileStr();

    HFileStr& operator=(const HFileStr&) = delete;

    HFileStr& operator=(HFileStr&&) = delete;

public:
    HRET InOpen(HCSTRR strFileName);

    HRET OutOpen(HCSTRR strFileName);

    HRET Open(HCSTRR strFileName, SYS_T flags = O_RDWR | O_CREAT);

    HRET ReadToString (HSTRR str);

    HRET WriteFromString(HCSTRR str);

};

}

#endif // __H_HUICPP_FILE_STRING_H__

