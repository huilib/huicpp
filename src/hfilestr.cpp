

#include "hfilestr.h"
#include <sstream>

namespace HUICPP {

HFileStr::HFileStr() 
    : base_t () {

}


HFileStr::HFileStr(HCSTRR strFileName, SYS_T flags) 
    : base_t(strFileName, flags) {

}


HFileStr::~ HFileStr() {

}


HRET HFileStr::InOpen(HCSTRR strFileName) {

    Close();    

    return Open(strFileName, O_RDONLY);

}


HRET HFileStr::OutOpen(HCSTRR strFileName) {

    Close();

    HNOTOK_RETURN(Open(strFileName, O_RDWR | O_CREAT));

    HNOTOK_RETURN(SetToZero());

    HRETURN_OK;

}


HRET HFileStr::Open(HCSTRR strFileName, SYS_T flags) {
    
    HNOTOK_RETURN(base_t::Open(strFileName, flags));

    HASSERT_RETURN(not IsInvalid(), ERR_STATUS);

    HRETURN_OK;

}


HRET HFileStr::ReadToString (HSTRR str) {

    HASSERT_RETURN(not IsInvalid(), ERR_STATUS);

    base_t::ReadString(str);

    HRETURN_OK;

}


HRET HFileStr::WriteFromString(HCSTRR str) {

    HASSERT_RETURN(not IsInvalid(), ERR_STATUS);

    base_t::WriteString(str);

    HRETURN_OK;

}


}