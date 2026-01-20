

#include "hthread.h"
#include "hutil.h"

namespace HUICPP {



HRET HThreadAttr::SetDetachState() noexcept {

	HASSERT_RETURN(
		pthread_attr_setdetachstate(GetPointer(), PTHREAD_CREATE_DETACHED) == 0,
		SYS_FAILED);

	HRETURN_OK;

}


HThread::HThread (const HThreadAttr& attr, thread_fun_t fun, void* arg) 
	: base() {

	if (fun != nullptr) {

		HIGNORE_RETURN(pthread_create(GetPointer(), attr.GetPointer(), fun, arg));

	}

}


HThread::~ HThread () {
	
}


HThread& HThread::operator=(HThread&& other) {

	base::operator=(std::move(other));
	return *this;

}


HRET HThread::Create(const HThreadAttr& attr, thread_fun_t fun, void* arg) {

	HASSERT_RETURN(fun != nullptr, INVL_PARA);

	HASSERT_RETURN(pthread_create(GetPointer(), attr.GetPointer(), fun, arg) == 0, SYS_FAILED);

	HRETURN_OK;

}

	
HRET HThread::Create(thread_fun_t fun, void* arg) {
	
	HASSERT_RETURN(fun != nullptr, INVL_PARA);

	HASSERT_RETURN(pthread_create(GetPointer(), nullptr, fun, arg) == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HThread::Cancel() {

	HASSERT_RETURN(pthread_cancel(GetId()) == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HThread::Join (void** rep) {

	HASSERT_RETURN(not IsInvalid(), ERR_STATUS);

	auto cb = pthread_join(GetId(), rep);
 	HASSERT_RETURN(cb == 0, SYS_FAILED);

 	SetInvalid();

	HRETURN_OK;
}


HRET HThread::JoinWithTime (const HTimeSpan& timespan, void** rep) {

	struct timespec _tv = {0,0};
	_tv.tv_sec = timespan.GetSeconds();
	_tv.tv_nsec = 0;
	
	auto cb = pthread_timedjoin_np(GetId(), rep, &_tv);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	SetInvalid();

	HRETURN_OK;

}


HRET HThread::Detach () const{

	HASSERT_RETURN(pthread_detach(GetId()) == 0, SYS_FAILED);

	HRETURN_OK;

}


HULL HThread::CurrentThreadId() {
	return static_cast<HULL>(pthread_self());
}


HSTR HThread::CurrentThreadHexIdStr() {
	HCH buf[9] = {0};
	snprintf(buf, 9, "%x", static_cast<HN>(pthread_self()));
	return HSTR(buf);
}


}


