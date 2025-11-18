
#ifndef __HUICPP_INNER_CURL_H__
#define __HUICPP_INNER_CURL_H__

#include <huicpp.h>
#include <curl/curl.h>


namespace HUICPP {

    namespace HTTP {

        namespace INNER_CURL {

            struct CURL_IMPL{
                CURL* handle;
            };

        }

    }

}



#endif //__INNER_CURL_H__