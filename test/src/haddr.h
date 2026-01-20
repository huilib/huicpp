/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp socket address.
*/


#ifndef __HADDR_H__
#define __HADDR_H__


#include <type_traits>

#include "huicpp.h"
#include "hsocket.h"

#include <netinet/in.h>
#include <sys/un.h>

#include "detail/network_detail.h"

namespace HUICPP {

class HSocket;


class HAddr {
public:
	constexpr static HN ADDR_DEFA_PORT = 0;

	enum class ADDR_TYPE {
	    IP_NORMAL,
	    IP_V4,
	    IP_V6,
		ADDR_UNIX,
		ADDR_END
	};
	
public:
    HAddr() noexcept = default;

    virtual ~ HAddr () noexcept = default;

public:
	virtual ADDR_TYPE GetAddrType () const {
		return ADDR_TYPE::IP_NORMAL;
    }

    virtual HRET Bind (const HSocket& _socket) const = 0;

    virtual HRET Connect (const HSocket& _socket) const = 0;

    virtual HRET Accept (const HSocket& server, HSocket& client) = 0;

    virtual HRET GetFromSocket (const HSocket& sock) = 0;
	
    virtual HRET GetAddrInfo (HSTRR strIp, HNR iPort) const = 0;	    

	virtual HSTR ToString () const = 0;

	virtual HOFF Recvfrom(const HSocket& sock, HPTR buf, HSIZE size, SYS_T flags) = 0;

	virtual HOFF Sendto(const HSocket& sock, HCPTR cbuf, HSIZE size, SYS_T flags) const = 0;
};


class HIp4Addr : public HAddr {	
public:
	using addr_integer_type = HUN;
	static constexpr addr_integer_type ADDR_DEFA_VALUE = 0;
	using sockaddr_type = struct sockaddr_in;

public:
	HIp4Addr() noexcept = default;

	HIp4Addr(addr_integer_type addr_bytes, HN _port = ADDR_DEFA_PORT);

	HIp4Addr(HCSTRR strIp, HN _port = ADDR_DEFA_PORT);

	explicit HIp4Addr(HCSTRR strIpPort);

	explicit HIp4Addr (const sockaddr_type& addr) ;

	HIp4Addr(const HIp4Addr&);

	virtual ~ HIp4Addr() noexcept = default;

public:
	HRET Setup(addr_integer_type addr_bytes = ADDR_DEFA_VALUE, HN _port = ADDR_DEFA_PORT);

	HRET Setup(HCSTRR strIp, HN _port);

	HRET Setup(const sockaddr_type& addr);

	HRET SetupPNet(HCSTRR strIp, HN port);

	void SetPort(HN port);

	const sockaddr_type& GetSockaddr() const { return m_addr; }

public:
	ADDR_TYPE GetAddrType () const override{
		return ADDR_TYPE::IP_V4;
	}

	HRET Bind (const HSocket& _sock) const override;

	HRET Connect (const HSocket& _sock) const override;

	HRET Accept (const HSocket& server, HSocket& client) override;

	HRET GetFromSocket (const HSocket& sock) override;

	HRET GetAddrInfo (HSTRR strIp, HNR iPort) const override;

	addr_integer_type GetIpInt () const;

	virtual HSTR ToString () const override;

	HOFF Recvfrom(const HSocket& sock, HPTR buf, HSIZE size, SYS_T flags) override;

	HOFF Sendto(const HSocket& sock, HCPTR cbuf, HSIZE size, SYS_T flags) const override;

	static HRET GetLocalIps (HVSTRR ips);

	static addr_integer_type GetIPByName (HCSTRR strName);

	static addr_integer_type GetNetMaskByName(HCSTRR strName);

	static addr_integer_type StringIpToInt(HCSTRR strIp);

	static addr_integer_type GetGateway(HCSTRR strName);

	static HSTR GetHwAddr(HCSTRR strName);

	static HSTR ToStringIp(addr_integer_type);

	static addr_integer_type AddrTypeToInt(in_addr_t iaddr);

	static HSTR GetLocalIp();

	static void GetLocalAddr(struct sockaddr_storage&);

public:
	static bool IsIp4Str(HCSTRR strIp);

	
private:
	sockaddr_type m_addr;
};


class HUnixAddr : public HAddr {	
public:
	using base = HAddr;
	using sockaddr_type = struct sockaddr_un;

public:
	HUnixAddr() noexcept = default;

	explicit HUnixAddr (HCSTRR strAddrPath);

	explicit HUnixAddr (const HUnixAddr& addr);	

	virtual ~ HUnixAddr() noexcept = default;

public:
	ADDR_TYPE GetAddrType () const override{
		return ADDR_TYPE::ADDR_UNIX;
	}

	HRET Bind (const HSocket& _sock) const override;

	HRET Connect (const HSocket& _sock) const override;

	HRET Accept (const HSocket& server, HSocket& client) override;

	HRET GetFromSocket (const HSocket& sock) override;

	HRET GetAddrInfo (HSTRR strIp, HNR iPort) const override;

	virtual HSTR ToString () const override;

	HOFF Recvfrom(const HSocket& sock, HPTR buf, HSIZE size, SYS_T flags) override;

	HOFF Sendto(const HSocket& sock, HCPTR cbuf, HSIZE size, SYS_T flags) const override;

protected:
	HRET init (HCSTRR strAddrPath);
	
private:
	sockaddr_type m_addr;
};

}

#endif //__HADDR_H__














