/*
 * @Author: Tom Hui
 * @Date: 2019-12-30 10:53:05
 * @Description: 
 */




#ifndef __HUICPP_HTTPHELPER_H__
#define __HUICPP_HTTPHELPER_H__


#include <huicpp.h>
#include <memory>


namespace HUICPP {

namespace HTTP {

	namespace INNER_CURL {
		struct CURL_IMPL;
	}

	class HCurl {
	public:
		typedef struct {
			HN CONNECTTIMEOUT = 3;
			HN TIMEOUT = 10;
		} CURL_INFO;
	public:
		HCurl ();
		HCurl (const HCurl& curl);
		HCurl (HCurl&& curl);		

		~ HCurl();

	public:		
		HRET Get (HCSTRR strUrl, HSTRR strRes);

		HRET Post (HCSTRR strUrl, HCSTRR strData, HSTRR strRes);

		void SetConnectTimeout (HN CONNECTTIMEOUT = 3) { m_info.CONNECTTIMEOUT = CONNECTTIMEOUT; }

		void SetTimeout (HN TIMEOUT = 10) { m_info.TIMEOUT = TIMEOUT; }

	private:
		// Re-initializes a CURL handle to the default values.
		RET_T reset ();

		void init_exec();

	private:
		INNER_CURL::CURL_IMPL* m_impl = nullptr;

		CURL_INFO m_info;
	};

}

}



#endif //__HTTPHELPER_H__


