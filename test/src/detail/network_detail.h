/*
 * Created: Joseph Hui Sat Sep 18 2021
 * Description: 
  		Wrapper for huicpp address function detail.
*/

#ifndef __H_HUICPP_NETWORK_DETAIL_H__
#define __H_HUICPP_NETWORK_DETAIL_H__


#include "../huicpp.h"
#include <netinet/in.h>


namespace HUICPP {

namespace detail {

class network_detail {
protected:
    network_detail() noexcept = default;
    ~network_detail () noexcept = default;

public:
    static HRET GetLocalIps4 (HVSTRR ip4s);

    static HRET GetLocalIps6 (HVSTRR ip6s);

    static in_addr_t GetIfAddr(HCSTRR strIfName);

    static in_addr_t GetNetMask(HCSTRR strIfName);

    /*
    *   get getway 
    */
    static in_addr_t GetGateWay(HCSTRR strIfName);

    static HSTR GetHwAddr (HCSTRR strName);

private:
    static HRET get_local_ips (HVSTRR ips, HN domain);

};

}

}


#endif //__H_HUICPP_NETWORK_DETAIL_H__
