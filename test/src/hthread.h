/*
 * Created: Joseph Hui Thu Sep 21 2017
 * Description: 
  		Wrapper for huicpp thread.
*/

#ifndef __H_HUICPP_THREAD_H__
#define __H_HUICPP_THREAD_H__

#include "huicpp.h"
#include <pthread.h>
#include "htime.h"
#include "internal/value_initor_in.h"

namespace HUICPP {

DEFINE_OTHER_TRAITS(pthread_attr_t,pthread_attr_init,pthread_attr_destroy);
using thread_attr_base = hc_internal::value_initor<pthread_attr_t>;


class HThreadAttr : public thread_attr_base {
public:
	HRET SetDetachState() noexcept;

};


template <class _Ty>
struct threadtype_traits {
	using native_type = _Ty;
	using native_pointer = native_type*;
	using const_native_pointer = const native_type*; 
	static const native_type invalid_threadtype_value = {0};

	static HSTR pid2s (native_type handle) {

		HWORD_BUF buf = {0};
		HSZ pos = buf;
		unsigned char* usz = (unsigned char*)(void*)(&handle);

		HSTR ret("0x");
		
		for (size_t i = 0; i < sizeof(handle); ++i) {
			sprintf(pos, "%02x", (unsigned)(usz[i]));
			pos += 2;
		}

		ret.assign(buf);
		return ret;

	}
};


template<class _Ty, class _Traits = threadtype_traits<_Ty> > 
class threadtype_base {
public:
	using traits_type = _Traits;
	using native_type = typename traits_type::native_type;
	using native_pointer = typename traits_type::native_pointer;
	using const_native_pointer = typename traits_type::const_native_pointer;

public:
	threadtype_base() noexcept : m_id(traits_type::invalid_threadtype_value) { }

	threadtype_base(const threadtype_base& ) = delete;

	threadtype_base(threadtype_base&& other) noexcept : m_id(other.m_id) 
	{ other.SetInvalid(); }

	virtual ~ threadtype_base() { }

	threadtype_base& operator= (const threadtype_base& ) = delete;

	threadtype_base& operator= (threadtype_base&& other) noexcept{
		m_id = other.m_id;
		other.SetInvalid();
		return *this;
	}

public:
	void SetInvalid() { m_id = traits_type::invalid_threadtype_value; }

	bool IsInvalid() const noexcept { return m_id == traits_type::invalid_threadtype_value; }

	native_type GetId () const { return m_id; }

	native_pointer GetPointer() { return &m_id; }
	const_native_pointer GetPointer() const { return &m_id; } 

	HSTR GetPidStr () const {
		return traits_type::pid2s(GetId());
	}

private:
	native_type m_id;
};


using thread_base = threadtype_base<pthread_t>;
class HThread : public thread_base {
public:
	using base = thread_base;
	using native_type = typename base::native_type;
	using native_pointer = typename base::native_pointer;
	using const_native_pointer = typename base::const_native_pointer;
	typedef void* (*thread_fun_t)(void*);

public:
	HThread() noexcept = default;

	HThread (const HThreadAttr& attr, thread_fun_t fun, void* arg) ;

	HThread(HThread&& other) noexcept = default;

	virtual ~ HThread ();

	HThread& operator=(const HThread&) = delete;

	HThread& operator=(HThread&& other);
	
public:
	HRET Create(const HThreadAttr& attr, thread_fun_t fun, void* arg);

	HRET Create(thread_fun_t fun, void* arg);

	HRET Cancel();

    HRET Join (void** rep = nullptr) ;

    HRET JoinWithTime (const HTimeSpan& timespan, void** rep = nullptr);

	HRET Detach () const;

public:
	static HULL CurrentThreadId();

	static HSTR CurrentThreadHexIdStr();
};

}


#endif //__H_HUICPP_THREAD_H__

