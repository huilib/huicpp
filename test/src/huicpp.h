/**********************************************************
 * Created: Joseph Hui,
 * Create Date: Apr-25-2021
 * Description: 
 *      Define some basic types for huicpp.
 * Include this header file for use huicpp.
 **********************************************************/


#ifndef _H_HUICPP_H_
#define _H_HUICPP_H_

#include "internal/constant_in.h"
#include "internal/error_in.h"


namespace  HUICPP {

	using huicpp_error_code_t = RET_T;

    extern huicpp_error_code_t g_hErr;
    
    HCSZ hbase_err (RET_T err);

	huicpp_error_code_t hlast_err () ;

}

#include "hexception.h"

#endif //_H_HUICPP_H_


