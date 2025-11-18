/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define some constant types.
 * 
 **********************************************************/

#ifndef __H_HUICPP_CONSTANT_IN_H__
#define __H_HUICPP_CONSTANT_IN_H__

#include "basictypes_in.h"
#include "commonmacro_in.h"

namespace HUICPP {

    // define common constant
    HSTATIC_CONST_VAL(HN) HWORD_LEN = 128;
    typedef HCH HWORD_BUF[HWORD_LEN];

#define DECLARE_LEN_BUF_WORD(nn,cc) HSTATIC_CONST_VAL(HN) H##nn##_LEN = HWORD_LEN * cc; \
    typedef HCH H##nn##_BUF[H##nn##_LEN]

    DECLARE_LEN_BUF_WORD(NAME, 2);

    DECLARE_LEN_BUF_WORD(PATH, 8);

    DECLARE_LEN_BUF_WORD(STRING, 16);

    DECLARE_LEN_BUF_WORD(PAGE, 64);

    DECLARE_LEN_BUF_WORD(SMALLBLOCK, 256);

    DECLARE_LEN_BUF_WORD(BLOCK, 1024);

#undef DECLARE_LEN_BUF_WORD

    HSTATIC_CONST_VAL(SYS_T) INVALID_FD = -1;

#define IS_VALID_FD(fd) (fd > 0)

#ifndef CLOSE_FD
#define __CLOSE_FD(fd) do {\
	if(IS_VALID_FD(fd)) {\
	    ::close((fd)); \
	    (fd) = INVALID_FD; \
	}}while(0)
#define CLOSE_FD(fd) __CLOSE_FD(fd)
#endif


#ifdef ARMJSPP
static constexpr const HUN HARDWARE_INTERFERENCE_SIZE = 64;
#else 
static constexpr const HUN HARDWARE_INTERFERENCE_SIZE = 128;
#endif
//static constexpr const HUN HARDWARE_INTERFERENCE_SIZE = 64;

}


#endif //__H_HUICPP_CONSTANT_IN_H__
