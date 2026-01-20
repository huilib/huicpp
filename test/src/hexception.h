/*
 * Created: Joseph Mar-29-2021
 * Description: 
  		Wrapper for huicpp exception. 
*/


#ifndef _H_HUICPP_EXCEPTION_H__
#define _H_HUICPP_EXCEPTION_H__

#include "huicpp.h"

namespace HUICPP {

class HException : public std::exception
{
public:
	HException (HN err = HTO_INT(ERR_NO(OK)), 
				HCSZ szFile = nullptr,
	             HN iLineNo = 0) noexcept;

	HException (HCSTRR strErrMsg, 
		HN err = HTO_INT(ERR_NO(OK)), 
		HCSZ szFile = nullptr,
	    HN iLineNo = 0) noexcept;

	virtual ~HException () noexcept;

public:
	static HSTR GetSysError(HN errnum = errno);

public:
	virtual HCSZ what() const noexcept override
	{ return m_str.c_str(); }

	virtual HN GetHErr () const noexcept { return m_err;}

private:
	void initmsg();

	void appendLocationInfo();

private:
	HSTR m_str;
	HN m_err;	
	HSTR m_strFile;
	HN m_iLineno;	
};



#ifndef HASSERT_THROW
#define _IN_HASSERT_THROW(ff,ee) do {	\
	if (HUNLIKELY(not (ff))) {	\
		throw HException(ee, __FILE__, __LINE__); \
	}	\
} while(0)
#define __HASSERT_THROW(ff,ee) _IN_HASSERT_THROW(ff,ERR_NO(ee))
#define HASSERT_THROW(ff,ee) __HASSERT_THROW(ff,ee)
#endif //HASSERT_THROW

#ifndef HASSERT_MSG_THROW
#define _IN_HASSERT_MSG_THROW(ff,msg,ee) do {	\
	if (not (ff)) {	\
		LOG_WARNING("["#ff"] throw exception{msg[%s] base-msg[%s]}", 	\
			HSTR(msg).c_str(), hbase_err(ee)); \
		throw HException(msg, ee, __FILE__, __LINE__);	\
	}	\
} while(0) 	
#define __HASSERT_MSG_THROW(ff,msg,ee) _IN_HASSERT_MSG_THROW(ff,msg,ERR_NO(ee))
#define HASSERT_MSG_THROW(ff,msg,ee) __HASSERT_MSG_THROW(ff,msg,ee)
#endif //HASSERT_MSG_THROW

#ifndef HNOTOK_THROW
#define __HNOTOK_THROW(seg) do {	\
	RET_T ret = (seg); 	\
	IF_NOTOK(HUNLIKELY(ret)) {	\
		throw HException(ret, __FILE__, __LINE__);	\
	}	\
} while(0)
#define HNOTOK_THROW(seg) __HNOTOK_THROW(seg)
#endif //HNOTOK_THROW

#ifndef HNOTOK_MSG_THROW
#define __HNOTOK_MSG_THROW(seg,msg) do {	\
	RET_T ret = (seg); 	\
	IF_NOTOK(ret) {	\
		LOG_WARNING("["#seg"] throw exception{msg[%s] base-msg[%s]}", 	\
			HSTR(msg).c_str(), hbase_err(ret)); \
		throw HException(msg, ret, __FILE__, __LINE__);	\
	}	\
} while(0)
#define HNOTOK_MSG_THROW(seg,msg) __HNOTOK_MSG_THROW(seg,msg)
#endif //HNOTOK_THROW

}

#endif //_H_HUICPP_EXCEPTION_H__
