/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define some common macro tools.
 * 
 **********************************************************/


#ifndef __H_HUICPP_COMMON_MACRO_IN_H__
#define __H_HUICPP_COMMON_MACRO_IN_H__

#include "basictypes_in.h"
#include <string.h>

namespace HUICPP {

// define a static-const type.
#ifndef HSTATIC_CONST_VAL
#define __STATIC_CONST_VAL(ty) static constexpr const ty
#define HSTATIC_CONST_VAL(ty) __STATIC_CONST_VAL(ty)
#endif // HSTATIC_CONST_VAL


// delete pointer helper;
#ifndef HDELP
#define __HDELP(ptr) do {\
		if((ptr) != nullptr) {\
			delete (ptr); \
			(ptr) = nullptr;\
	}} while(0)
#define HDELP(ptr) __HDELP(ptr)  
#endif //HDELP


// delete array pointer helper;
#ifndef HDEL_ARR
#define __HDEL_ARR(lp) do { \
	if ((lp) != nullptr) { \
	    delete [] (lp); \
	    (lp) = nullptr; \
	} \
} while(0) 
#define HDEL_ARR(lp) __HDEL_ARR(lp)
#endif ///HDEL_ARR


// free helper;
#ifndef HFREE
#define __HFREE(ptr) do { \
	if ((ptr) != nullptr) { \
		free((ptr)); \
		(ptr) = nullptr; \
	} \
} while(0)
#define HFREE(ptr) __HFREE(ptr)
#endif //__HFREE


// static cast to int.
#ifndef HTO_INT
#define __HTO_INT(n) static_cast<HN>(n)
#define HTO_INT(n) __HTO_INT(n)
#endif


// set zero to memory.
#ifndef ZERO_MEM
#define __ZERO_MEM(buf, len) memset((buf), 0, (len))
#define ZERO_MEM(buf, len) __ZERO_MEM(buf, len)    
#endif   


// null pointer for `new` returned, we return memory-out.
#ifndef NULLPOINTER_CHECK
#define __NULLPOINTER_CHECK(ptr) HASSERT_RETURN((ptr) != nullptr, MEMORY_OUT)
#define NULLPOINTER_CHECK(ptr) __NULLPOINTER_CHECK(ptr)
#endif // NULLPOINTER_CHECK


// check new pointer that make by `new`;
#ifndef CHECK_NEWPOINT
#define __CHECK_NEWPOINT(p) HASSERT_THROW((p)!=nullptr, MEMORY_OUT)
#define CHECK_NEWPOINT(p) __CHECK_NEWPOINT(p)
#endif //CHECK_NEWPOINT


// define a return-type that make user check the value.
#ifndef HRET
#define __HRET __attribute__((warn_unused_result)) RET_T
#define HRET __HRET
#endif //RET_T


// ignore return value helper.
#ifndef HIGNORE_RETURN
#define _HIGNORE_RETURN(ss) do{	\
	auto _ret = (ss); \
	(void) _ret;	\
} while(0) 
#define HIGNORE_RETURN(ss) _HIGNORE_RETURN(ss)
#endif //HIGNORE_RETURN


// to-undo helper holder generator.
#ifndef HOLDER_GENERATE
#define _HOLDER_GENERATE(ty,dosth,undosth) class  ty ## _ ## dosth ## _ ## undosth {	\
public:	\
	explicit ty ## _ ## dosth ## _ ## undosth ( ty & __m): m_device(__m) {	\
		HIGNORE_RETURN(m_device. dosth ());	\
	}	\
	~ ty ## _ ## dosth ## _ ## undosth ()	\
    { HIGNORE_RETURN(m_device. undosth ()); }	\
    ty ## _ ## dosth ## _ ## undosth (const ty ## _ ## dosth ## _ ## undosth &) = delete;	\
    ty ## _ ## dosth ## _ ## undosth & operator=(const ty ## _ ## dosth ## _ ## undosth &) = delete; \
private:	\
	ty & m_device;	\
}
#define HOLDER_GENERATE(t,d,u) _HOLDER_GENERATE(t,d,u)
#endif //HOLDER_GENERATE

#ifndef HNORETURN_EXIT
#define HNORETURN_EXIT [[noreturn]]
#endif // HNORETURN_EXIT

#ifdef __GNUC__
#define HLIKELY(x) (__builtin_expect((x), 1))
#define HUNLIKELY(x) (__builtin_expect((x), 0))
#else
#define HLIKELY(x) (x)
#define HUNLIKELY(x) (x)
#endif // __GNUC__

#ifndef HCONCATNAME
#define HCONCATNAME_IMPL(s1,s2) s1##s2
#define HCONCATNAME(s1,s2) HCONCATNAME_IMPL(s1,s2)
#endif // HCONCATNAME

#ifndef HANONYMOUS_VARIABLE
#define HANONYMOUS_VARIABLE_IMPL(str) \
	HCONCATNAME(HCONCATNAME(HCONCATNAME(str,__COUNTER__),__),__LINE__)
#define HANONYMOUS_VARIABLE(str) HANONYMOUS_VARIABLE_IMPL(str)
#endif	//HANONYMOUS_VARIABLE


// huicpp common catch exception statements.
#ifndef HCATCH_EXCEPTION_STATE
#define _HCATCH_EXCEPTION_STATE_IMPL(appname) \
catch (const HException& hex) {	\
	LOG_ERROR(#appname" get a huicpp exception:[%s]", hex.what()); 	\
	return false;	\
} catch (const std::exception& sex) {	\
	LOG_ERROR(#appname" get a standard exception:[%s]", sex.what());	\
	return false; 	\
} catch (...) {	\
	LOG_ERROR(#appname" get a unknown exception");	\
	return false;	\
}
#define HCATCH_EXCEPTION_STATE(appname) _HCATCH_EXCEPTION_STATE_IMPL(appname)
#endif //HCATCH_EXCEPTION_STATE

}

#include "gcc_attributes_in.h"

#endif //__H_HUICPP_COMMON_MACRO_IN_H__

