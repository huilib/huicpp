

#include <netdb.h>
#include "haddr.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

using namespace HUICPP;

namespace {

bool is_bad_address4_for_us(HUICPP::HIp4Addr::addr_integer_type addr) {
	HUICPP::HIp4Addr::addr_integer_type nAddr = htonl(addr);
	return (nAddr == 0x7F000001 /* 127.0.0.1 */
		|| nAddr == 0
		|| nAddr == (HUICPP::HIp4Addr::addr_integer_type)(~0));
}


bool isBadAddressForUs(const struct sockaddr& addr){

	switch (addr.sa_family)
	{
	case AF_INET:
		return is_bad_address4_for_us(((sockaddr_in&)addr).sin_addr.s_addr);
		break;

	case AF_INET6:
		// IP6 UNSUPPORT
		break;
	
	default:
		break;
	}

	return true;

}

bool addressIsNull(const sockaddr_storage& address) {
	switch (address.ss_family) {
		case AF_INET: {
			return ((sockaddr_in const&)address).sin_addr.s_addr == 0;
		}
		case AF_INET6: {
			for (unsigned i = 0; i < 16; ++i) {
				if (((const sockaddr_in6&)address).sin6_addr.s6_addr[i] != 0) {
					return false;
				}
			}
			return true;
		}
		default: {
			return false;
		}
	}

	return false;
}

void copyAddress(struct sockaddr_storage& to, struct sockaddr const* from) {
	// Copy a "struct sockaddr" to a "struct sockaddr_storage" (assumed to be large enough)
	if (from == nullptr) {
		return;
	}
	
	switch (from->sa_family) {
		case AF_INET: {
#ifdef HAVE_SOCKADDR_LEN
			if (from->sa_len < sizeof (struct sockaddr_in)) return; // sanity check
			to.ss_len = sizeof (struct sockaddr_in);
#endif
			to.ss_family = AF_INET;
			((sockaddr_in&)to).sin_addr.s_addr = ((sockaddr_in const*)from)->sin_addr.s_addr;
			break;
		}
		case AF_INET6: {
#ifdef HAVE_SOCKADDR_LEN
		if (from->sa_len < sizeof (struct sockaddr_in6)) return; // sanity check
		to.ss_len = sizeof (struct sockaddr_in6);
#endif
			to.ss_family = AF_INET6;
			for (unsigned i = 0; i < 16; ++i) {
				((sockaddr_in6&)to).sin6_addr.s6_addr[i] = ((sockaddr_in6 const*)from)->sin6_addr.s6_addr[i];
			}
			break;
		}
	}
}

void setupNet4WithIpString(HCSTRR strIp, HN port, HIp4Addr::sockaddr_type& net) {

	net.sin_family = AF_INET;
	net.sin_port = htons(port);
	net.sin_addr.s_addr = inet_addr(strIp.c_str());

}


HRET parseNet4FromString (HCSTRR str, HIp4Addr::sockaddr_type& net) {

	memset(&net, 0, sizeof(net));
	
	// 127.0.0.1:809
	HCSTR ip_split = ":";
	HSIZE pos = str.find(ip_split);
	if (pos == HStr::npos || pos == str.length() -1) {
		HRETURN(INVL_PARA);
	}

	auto is_ip_str = [] (HCSTRR ip) ->bool {
		return ip.find_first_not_of("1234567890.") == HSTR::npos;
	};

	HSTR ssip = str.substr(0, pos);
	if (!is_ip_str(ssip)) {
		HRETURN(INVL_PARA);
	}

	HN iPort = HStr(str.substr(pos+1)).ToN();

	setupNet4WithIpString(ssip, iPort, net);

	HRETURN_OK;
	
}


}


namespace HUICPP {


HIp4Addr::HIp4Addr(addr_integer_type addr_bytes, HN _port)
	: m_addr() {

	HIGNORE_RETURN(Setup(addr_bytes, _port));

}


HIp4Addr::HIp4Addr(HCSTRR strIp, HN _port) 
	: m_addr() {

	HIGNORE_RETURN(Setup(strIp, _port));
	
}


HIp4Addr::HIp4Addr(HCSTRR strIpPort) 
	: m_addr() {

	ZERO_MEM(&m_addr, sizeof(m_addr));
	HIGNORE_RETURN(parseNet4FromString(strIpPort, m_addr));
	
}


HIp4Addr::HIp4Addr(const sockaddr_type& addr) 
	: m_addr() {

	HIGNORE_RETURN(Setup(addr));

}


HIp4Addr::HIp4Addr(const HIp4Addr& other) 
	: m_addr() {

	HIGNORE_RETURN(Setup(other.m_addr));

}


HRET HIp4Addr::Setup(addr_integer_type addr_bytes, HN _port) {

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(_port);
	memcpy(&m_addr.sin_addr.s_addr, &addr_bytes, sizeof(m_addr.sin_addr.s_addr));

	HRETURN_OK;
}


HRET HIp4Addr::Setup(const sockaddr_type& addr) {

	memcpy(&m_addr, &addr, sizeof(m_addr));
	HRETURN_OK;

}


HRET HIp4Addr::Setup(HCSTRR strIp, HN _port) {

	setupNet4WithIpString(strIp, _port, m_addr);
	
	HRETURN_OK;

}


HRET HIp4Addr::SetupPNet(HCSTRR strIp, HN port) {

	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);

	int ret = inet_pton(AF_INET, strIp.c_str(), &m_addr.sin_addr);
	if (ret == 1) {
		HRETURN_OK;
	}

	if (ret == 0) {
		HRETURN(INVL_PARA);
	}

	HRETURN(SYS_FAILED);

}


void HIp4Addr::SetPort(HN port) {

	m_addr.sin_port = htons(port);

}


HRET HIp4Addr::Bind (const HSocket& _sock) const {

	SYS_RET_T cb = ::bind(_sock.Fd(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
	
}


HRET HIp4Addr::Connect (const HSocket& _sock) const {

	SYS_RET_T cb = ::connect(_sock.Fd(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;

}


HRET HIp4Addr::Accept (const HSocket& server, HSocket& client) {

	socklen_t len = sizeof(m_addr);
	SYS_T cb = ::accept(server.Fd(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb > 0, SYS_FAILED);

	client.SetFd(cb);

	HRETURN_OK;
}


HRET HIp4Addr::GetFromSocket (const HSocket& sock) {

	socklen_t len = sizeof(m_addr);
	SYS_RET_T cb = getpeername(sock.Fd(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HRET HIp4Addr::GetAddrInfo (HSTRR strIp, HNR iPort) const {

	strIp = inet_ntoa(m_addr.sin_addr);
	iPort = ntohs(m_addr.sin_port);

	HRETURN_OK;

}


HIp4Addr::addr_integer_type HIp4Addr::GetIpInt () const {

	return inet_lnaof(m_addr.sin_addr);

}


HSTR HIp4Addr::ToString () const {

	HSTR strIp;
	HN port = ADDR_DEFA_PORT;
	
	IF_NOTOK(GetAddrInfo(strIp, port)) {
		return "";
	}

	return HStr::Format("%s:%d", strIp.c_str(), port);
}


HOFF HIp4Addr::Recvfrom(const HSocket& sock, HPTR buf, HSIZE size, SYS_T flags) {
	socklen_t len = sizeof(m_addr);
	return ::recvfrom(sock.Fd(), buf, size, flags, 
		reinterpret_cast<struct sockaddr*>(&m_addr), &len);
}


HOFF HIp4Addr::Sendto(const HSocket& sock, HCPTR cbuf, HSIZE size, SYS_T flags) const {
	socklen_t len = sizeof(m_addr);
	return ::sendto(sock.Fd(), cbuf, size, flags, 
		reinterpret_cast<const struct sockaddr*>(&m_addr), len);
}


HRET HIp4Addr::GetLocalIps (HVSTRR ips) {
	return detail::network_detail::GetLocalIps4(ips);
}


HIp4Addr::addr_integer_type HIp4Addr::GetIPByName (HCSTRR strName) {
	return AddrTypeToInt(detail::network_detail::GetIfAddr(strName));
}

HIp4Addr::addr_integer_type HIp4Addr::GetNetMaskByName(HCSTRR strName) {
	return AddrTypeToInt(detail::network_detail::GetNetMask(strName));
}

HIp4Addr::addr_integer_type HIp4Addr::StringIpToInt(HCSTRR strIp) {
	in_addr_t iaddr = inet_addr(strIp.c_str());
	return ntohl(iaddr);
}

HIp4Addr::addr_integer_type HIp4Addr::GetGateway(HCSTRR strName) {
	return AddrTypeToInt(detail::network_detail::GetGateWay(strName));
}

HSTR HIp4Addr::GetHwAddr(HCSTRR strName) {
	return detail::network_detail::GetHwAddr(strName);
}

HSTR HIp4Addr::ToStringIp(addr_integer_type ll) {
	struct in_addr addr;
	addr.s_addr = htonl(ll);
	return HSTR(inet_ntoa(addr));
}

HIp4Addr::addr_integer_type HIp4Addr::AddrTypeToInt(in_addr_t iaddr) {
	return static_cast<addr_integer_type>(htonl(iaddr));
}


HSTR HIp4Addr::GetLocalIp() {

	struct sockaddr_storage ss;
	GetLocalAddr(ss);
	
	return HSTR(inet_ntoa(((sockaddr_in&)ss).sin_addr));

}


void HIp4Addr::GetLocalAddr(struct sockaddr_storage& ss) {
	// We use two methods to (try to) get our IP addresses.
	// First, we use "getifaddrs()". But if that doesn't work,
	// (or if "getifaddrs()" is not defined), then we use an alternative
	// (more -old-fashion) mechanism: First get our host name,
	// then try resolving this host name.
	ss.ss_family = AF_INET;
	((sockaddr_in&)ss).sin_addr.s_addr = 0;

	//bool getifaddrsWorks = false;

	struct ifaddrs* ifap;
	if (getifaddrs(&ifap) == 0) {
		
		for (struct ifaddrs* p = ifap; p != nullptr; p = p->ifa_next) {
			
			if ((p->ifa_flags & IFF_UP) == 0 || (p->ifa_flags & IFF_LOOPBACK) != 0) {
				// Ignore an interface if it's not up, or is a loopback interface:
				continue;
			}
		
			if (p->ifa_addr == NULL || isBadAddressForUs(*p->ifa_addr)) {
				// Also ignore the interface if the address is considered 'bad' for us:
				continue;
			}
      
			// We take the first IPv4 and first IPv6 addresses:
			if (p->ifa_addr->sa_family == AF_INET and addressIsNull(ss)) {
				copyAddress(ss, p->ifa_addr);
				//getifaddrsWorks = true;
			} 
			/*else if (p->ifa_addr->sa_family == AF_INET6 && addressIsNull(foundIPv6Address)) {
			copyAddress(foundIPv6Address, p->ifa_addr);
			getifaddrsWorks = True;
			}*/
		}

		freeifaddrs(ifap);
	}

	/*if (not getifaddrsWorks) {

		do {
			// We couldn't find our address using "getifaddrs()",
			// so try instead to look it up directly - by first getting our host name,
			// and then resolving this host name
			char hostname[100];
			hostname[0] = '\0';
			int result = gethostname(hostname, sizeof hostname);
			if (result != 0 || hostname[0] == '\0') {
				//env.setResultErrMsg("initial gethostname() failed");
				break;
			}

			// Try to resolve "hostname" to one or more IP addresses:
			NetAddressList addresses(hostname);
			NetAddressList::Iterator iter(addresses);

			// Look at each address, rejecting any that are bad.
			// We take the first IPv4 and first IPv6 addresses, if any.
			NetAddress const* address;
			while ((address = iter.nextAddress()) != NULL) {
				if (isBadAddressForUs(*address)) continue;

				if (address->length() == sizeof (ipv4AddressBits) && addressIsNull(foundIPv4Address)) {
	  				copyAddress(foundIPv4Address, address);
				} else if (address->length() == sizeof (ipv6AddressBits) && addressIsNull(foundIPv6Address)) {
	  				copyAddress(foundIPv6Address, address);
				}
      		}
    	} while (0);

	}*/

}


bool HIp4Addr::IsIp4Str(HCSTRR strIp) {

	//xxx.xxx.xxx.xxx
	if (strIp.length() > 15) {
		return false;
	}

	auto ip_num = [] (HN n) -> bool {
		return n >= 0 and n <= 255;
	};

	HVSTR ncs;
	// split ip string, get numbers.
	IF_NOTOK(HStr(strIp).Split(".", ncs)) {
		return false;
	}

	// make sure there are three number strings.
	if (ncs.size() != 4) {
		return false;
	}

	for (HVSTR::size_type i = 0; i < ncs.size(); ++i) {
		HCSTRR strnum = ncs[i];
		// all number string must be a number.
		if (not HStr::IsAllDigit(strnum)) {
			return false;
		}

		HN n = HStr(strnum).ToN();
		if (not ip_num(n)) {
			return false;
		}
	}

	return true;
}

HUnixAddr::HUnixAddr (HCSTRR strAddrPath) 
	: base() {
	HIGNORE_RETURN(init(strAddrPath));
}

HUnixAddr::HUnixAddr(const HUnixAddr& addr)
	: base () {
	memcpy(&m_addr, &addr.m_addr, sizeof(m_addr));
}

HRET HUnixAddr::Bind (const HSocket& _sock) const {
	socklen_t len = sizeof(m_addr);
	HASSERT_RETURN(bind(_sock.Fd(), reinterpret_cast<const struct sockaddr*>(&m_addr), len)== 0, SYS_FAILED);
	HRETURN_OK;
}

HRET HUnixAddr::Connect (const HSocket&) const {
	throw HException("unix socket don't support connect", ERR_NO(UN_SUPPORT), __FILE__, __LINE__);
	HRETURN_OK;
}

HRET HUnixAddr::Accept (const HSocket&, HSocket&) {
	throw HException("unix socket don't support Accept", ERR_NO(UN_SUPPORT), __FILE__, __LINE__);
	HRETURN_OK;
}

HRET HUnixAddr::GetFromSocket (const HSocket& sock)  {
	socklen_t len = sizeof(m_addr);
	HASSERT_RETURN(getpeername(sock.Fd(), (struct sockaddr*)&m_addr, &len) == 0, SYS_FAILED);
	HRETURN_OK;
}

HRET HUnixAddr::GetAddrInfo (HSTRR, HNR) const {
	throw HException("unix socket don't support GetAddrInfo", ERR_NO(UN_SUPPORT), __FILE__, __LINE__);
	HRETURN_OK;
}

HSTR HUnixAddr::ToString () const {
	throw HException("unix socket don't support ToString", ERR_NO(UN_SUPPORT), __FILE__, __LINE__);
	return HSTR();
}

HOFF HUnixAddr::Recvfrom(const HSocket& sock, HPTR buf, HSIZE size, SYS_T flags) {
	socklen_t len = 0;
	return ::recvfrom(sock.Fd(), buf, size, flags, 
		reinterpret_cast<struct sockaddr*>(&m_addr), &len);
}

HOFF HUnixAddr::Sendto(const HSocket& sock, HCPTR cbuf, HSIZE size, SYS_T flags) const {
	socklen_t len = sizeof(m_addr);
	return ::sendto(sock.Fd(), cbuf, size, flags, 
		reinterpret_cast<const struct sockaddr*>(&m_addr), len);
}

HRET HUnixAddr::init (HCSTRR strAddrPath) {
	(void)unlink(strAddrPath.c_str());
	bzero(&m_addr, sizeof(m_addr));
	m_addr.sun_family = AF_UNIX;
	strcpy(m_addr.sun_path, strAddrPath.c_str());
	HRETURN_OK;
}

}

