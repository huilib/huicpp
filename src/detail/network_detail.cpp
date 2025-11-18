

#include "network_detail.h"
#include "../htodoholder.h"
#include <cstdio>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/route.h>   
#include <linux/netdevice.h> 
#include <unistd.h>

namespace HUICPP {

namespace detail {


namespace {

struct ifreq get_ifaddr_by_name(HCSTRR strIfName, SYS_T flag) {

    struct ifreq ifr;
    ZERO_MEM(&ifr, sizeof(struct ifreq));

    SYS_T sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock <= 0) {
        return ifr;
    }
    HSCOPE_EXIT { close(sock); };

    strncpy(ifr.ifr_name, strIfName.c_str(), IFNAMSIZ - 1);

    if (ioctl(sock, flag, &ifr) < 0) {
        return ifr;
    }

    return ifr;
}

}


HRET network_detail::GetLocalIps4 (HVSTRR ip4s) {

    return get_local_ips(ip4s, AF_INET);

}



HRET network_detail::GetLocalIps6 (HVSTRR ip6s) {

    return get_local_ips(ip6s, AF_INET6);

}


in_addr_t network_detail::GetIfAddr(HCSTRR strIfName) {
    struct ifreq ifr = get_ifaddr_by_name(strIfName, SIOCGIFADDR);
    struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
    return addr->sin_addr.s_addr; 
}


in_addr_t network_detail::GetNetMask(HCSTRR strIfName) {
    struct ifreq ifr = get_ifaddr_by_name(strIfName, SIOCGIFNETMASK);
    struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
    return addr->sin_addr.s_addr; 
}


in_addr_t network_detail::GetGateWay(HCSTRR strIfName) {

    HNAME_BUF buf = {0};
    FILE* pf = fopen("/proc/net/route", "r");
    if (pf == nullptr) {
        return INADDR_ANY;
    }
    HSCOPE_EXIT { fclose(pf); };
    fgets(buf, HNAME_LEN, pf);

    HWORD_BUF word = {0};
    unsigned long dest = 0, gate = 0, flags = 0;

    ZERO_MEM(buf, HNAME_LEN);
    while(fgets(buf, HNAME_LEN, pf) != nullptr) {

        HSCOPE_EXIT { ZERO_MEM(buf, HNAME_LEN); };
        ZERO_MEM(word, HWORD_LEN);
        (void)sscanf(buf, "%s\t%08lX\t%08lX\t%08lX\t", word, &dest, &gate, &flags);
        HSTR strName(word);
        if (strName != strIfName) {        
            continue;
        }

        if ((flags & (RTF_UP | RTF_GATEWAY)) == (RTF_UP | RTF_GATEWAY)) {
            return gate;
        }

    }

    return INADDR_ANY;
}


HSTR network_detail::GetHwAddr (HCSTRR strName) {
    struct ifreq ifr = get_ifaddr_by_name(strName, SIOCGIFHWADDR);
    return HSTR(ifr.ifr_ifru.ifru_hwaddr.sa_data, IFHWADDRLEN);
}


HRET network_detail::get_local_ips (HVSTRR ips, HN domain) {

    struct ifconf ifconf;
    ZERO_MEM(&ifconf, sizeof(struct ifconf));
    struct ifreq ifr[50];
    ZERO_MEM(ifr, sizeof(ifr));
    ifconf.ifc_buf = reinterpret_cast<char*>(ifr);
    ifconf.ifc_len = sizeof(ifr);

    int s = socket(domain, SOCK_STREAM, 0);
    HASSERT_RETURN(s > 0, SYS_FAILED);

    HASSERT_RETURN(ioctl(s, SIOCGIFCONF, &ifconf) == 0, SYS_FAILED);

    int ifs = ifconf.ifc_len / sizeof(ifr[0]);
    ips.clear();

    for (int i = 0; i < ifs; i++) {
        char ip[INET_ADDRSTRLEN] = {0};
        struct sockaddr_in *s_in = reinterpret_cast<struct sockaddr_in *>(&ifr[i].ifr_addr);

        HASSERT_RETURN(inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip)) != nullptr, SYS_FAILED);
        ips.push_back(ip);
    }

    close(s);

    HRETURN_OK;

}


}

}