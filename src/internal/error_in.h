/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define huicpp error codes.
 * 
 **********************************************************/


#ifndef __H_HUICPP_ERROR_IN_H__
#define __H_HUICPP_ERROR_IN_H__

namespace HUICPP {

        // error map for hbase library
#define HUICPP_ERR_MAP(XX) \
    XX(OK, "success")	\
    XX(INVL_PARA, "invaild parameter") \
    XX(ERR_STATUS,"invalid status") \
	XX(SRC_ERR, "source error")	\
    XX(OVER_FLOW, "index overflow") \
    XX(IO_FAILED, "I/O failed") \
    XX(MEMORY_OUT, "memory out") \
    XX(TIME_OUT, "time out") \
    XX(NOT_EXISTS, "object not exists") \
    XX(DEP_ERROR, "dep source rise error") \
	XX(NETWORK_ERROR, "network error") \
    XX(SYS_FAILED, "syscall failed")	\
	XX(UN_SUPPORT, "unsupport")	\
    XX(UNKOWN_ERR, "unknown error number.") \
	\
    XX(ERR_END, "err-end") 


#define ERR_GEN(n,s) HUICPP_ERR_##n,
    enum huicpp_errs_enum{
		HUICPP_ERR_MAP(ERR_GEN)
    };
#undef ERR_GEN


#ifndef ERR_NO
#define _ERR_NO(err) huicpp_errs_enum::HUICPP_ERR_##err 
#define ERR_NO(err) _ERR_NO(err)
#endif


#ifndef IS_OK
#define _IS_OK(cb) ((cb) == ERR_NO(OK))
#define IS_OK(cb) _IS_OK(cb)

#define _NOT_OK(cb) ((cb) != ERR_NO(OK))
#define NOT_OK(cb) _NOT_OK(cb)

#define _IF_OK(cb) if (IS_OK(cb))
#define IF_OK(cb) _IF_OK(cb)

#define _IF_NOTOK(cb) if(NOT_OK(cb))
#define IF_NOTOK(cb) _IF_NOTOK(cb)
#endif

#ifndef HASSERT_RETURN
#define __HASSERT_RETURN(ff,ee) do {    \
	if (not (ff)) {     \
		HRETURN(ee); \
	}   \
} while(0)
#define HASSERT_RETURN(ff,ee) __HASSERT_RETURN(ff,ee)
#endif //HASSERT_RETURN

#ifndef HASSERT_MSG_RETURN
#define __HASSERT_MSG_RETURN(ff,msg,ee) do {\
	if (not (ff)) {\
		LOG_WARNING("["#ff"] failed msg[%s] base-msg[%s]",  \
            HSTR(msg).c_str(), hbase_err(ERR_NO(ee))); \
		HRETURN(ee);    \
	}\
} while(0)
#define HASSERT_MSG_RETURN(ff,msg,ee) __HASSERT_MSG_RETURN(ff,msg,ee)
#endif //HASSERT_MSG_RETURN

#ifndef HNOTOK_RETURN
#define __HNOTOK_RETURN(seg) do { \
	RET_T ret = (seg);	\
	IF_NOTOK(ret) {	\
		return ret; \
	} \
} while(0)
#define HNOTOK_RETURN(seg) __HNOTOK_RETURN(seg)
#endif //HASSERT_RETURN

#ifndef HNOTOK_MSG_RETURN
#define __HNOTOK_MSG_RETURN(seg,msg) do { \
	RET_T ret = (seg);	\
	IF_NOTOK(ret) {	\
		LOG_WARNING("["#seg"] failed msg[%s] base-msg[%s]", \
            HSTR(msg).c_str(), hbase_err(ret)); \
		return ret; \
	} \
} while(0)	
#define HNOTOK_MSG_RETURN(seg,msg) __HNOTOK_MSG_RETURN(seg,msg)
#endif

#ifndef HRETURN
#define _HRETURN(ret) if (!(IS_OK(ret))){	\
	    g_hErr = HTO_INT(ret);		\
	} \
    return ret

#define _HRETURN_(ret) _HRETURN(ret)
#define HRETURN(err_code) _HRETURN_(ERR_NO(err_code))
#endif

#ifndef HRETURN_OK
#define HRETURN_OK return ERR_NO(OK);
#endif

}

#endif //__H_HUICPP_ERROR_IN_H__
