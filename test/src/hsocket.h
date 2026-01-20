/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp socket.
*/


#ifndef __H_HUICPP_SOCKET_H__
#define __H_HUICPP_SOCKET_H__

#include "internal/fd_in.h"
#include "hstr.h"
#include "haddr.h"
#include "htime.h"
#include <netinet/tcp.h>
#include <unistd.h>

namespace HUICPP {

	class HSocketException;
    class HSocket;
    class HTcpSocket;
    class HUdpSock;
    class HAddr;
	class HIp4Addr;
	class HTcpClient;

#define RETURN_SOCK_STATUS HASSERT_RETURN(IsGoodSocket(), ERR_STATUS)

#define SOCK_FD_CHECK 	do{ if (not IsGoodSocket()) { \
		throw HSocketException(HStr::Format("It not a good socket. Socket:[%d]", Fd()), __FILE__, __LINE__); \
	} } while(0)

#define THROW_NET_EXCEPTION(ss) throw HSocketException(ss, __FILE__, __LINE__)


class HSocketException : public HException {
public:
	HSocketException(HStr strError, HCSTRR szFile = HSTR(), HN iLineNo = -1)
		: HException(strError, ERR_NO(NETWORK_ERROR), szFile.c_str(), iLineNo) {}
};


class HSocket : public hc_internal::iofd_in  {
public:
    using base = hc_internal::iofd_in;
	using fd_t = base::fd_t;
	constexpr static HN SOCKET_KEEPALIVE_DEF = 1;
	constexpr static HN SOCKET_KEEPIDLE_DEF = 600;
	constexpr static HN SOCKET_KEEPINTERVAL_DEF = 5;
	constexpr static HN SOCKET_KEEPCOUNT_DEF = 3;
    constexpr static HN SOCKET_DEFAULT_TIMEOUT = 1500;

    enum class SOCK_TYPE {
        ST_NORMAL,
        ST_TCP,
        ST_UDP,
        ST_UNIX,
        ST_END
    };

public:
    HSocket () noexcept
	    : base () { }

    explicit HSocket (fd_t sock) noexcept
	    : base (sock) { }

    HSocket (const HSocket&) = delete;

    // move construction
    HSocket (HSocket&& sock) noexcept = default;

    virtual ~HSocket() noexcept = default;

    HSocket& operator= (const HSocket& ) = delete;

    HSocket& operator= (HSocket&& sock) noexcept = default;

public:
    virtual HRET Init () = 0;   

    HRET Bind (const HAddr& addr) const ; 

    HRET Connect (const HAddr& addr) const;

    HRET GetAddrInfo (HAddr& addr) const;

    virtual HRET GetAddr4Info (HSTRR strIp, HNR iPort) const;

    virtual HSTR ToString () const;

public:
    HOFF Pread(HPTR, HSIZE, HOFF) const override;

    HOFF Pwrite(HCPTR, HSIZE, HOFF) const override;

    virtual HOFF WriteAll (HCPTR buf, HSIZE nbyte, 
        HN timeout = SOCKET_DEFAULT_TIMEOUT) const noexcept(false); 

    virtual HOFF ReadWithTimeOut (HPTR buf, 
        HSIZE nbyte, HN timeout = SOCKET_DEFAULT_TIMEOUT) const noexcept(false);

    HOFF ReadAllWithTimeout (HPTR buf, HSIZE nbyte, HN timeout = SOCKET_DEFAULT_TIMEOUT) const;

    virtual HOFF Send (HCPTR buf, HSIZE len, SYS_T flags = 0) const;

    virtual HOFF SendAll (HCPTR buf, HSIZE len, 
        SYS_T flags = 0, HN timeout = SOCKET_DEFAULT_TIMEOUT) const noexcept(false);

    virtual HOFF Recv (HPTR buf, HSIZE len, SYS_T flags = 0) const ;

    virtual HOFF RecvWithTimeOut (HPTR buf, HSIZE len, 
        SYS_T flags = 0, HN timeout = SOCKET_DEFAULT_TIMEOUT) const noexcept(false);
    
    bool IsGoodSocket() const noexcept { return  not IsInvalid(); }


    HRET SetSockOpt (SYS_T optName, HCPTR optVal,
		     socklen_t optLen, SYS_T level = SOL_SOCKET) const;


    HRET GetSockOpt (SYS_T optName, HPTR optVal,
		     socklen_t* optLen, SYS_T level = SOL_SOCKET) const;


    HRET SetReuseAddr () const;

    HRET SetReusePort () const;

    HRET GetSendBufferLength(HN & buf_len) const;
    
    HRET SetupSendBufLength(HN buf_len = 2 * 1024 * 1024) const;

    HRET GetRecvBufferLength(HN & buf_len) const;
    
    HRET SetupRecvBufLength(HN buf_len = 2 * 1024 * 1024) const;    

    virtual SOCK_TYPE GetSocketType () const noexcept {
	    return SOCK_TYPE::ST_NORMAL;
    }

};

class HTcpSocket : public HSocket {
public:
    using base = HSocket;

public:
    HTcpSocket () noexcept
	    : base () { }

    explicit HTcpSocket (fd_t sock) noexcept
	    : base (sock) { }

    HTcpSocket(const HTcpSocket& sock) = delete;

    HTcpSocket(HTcpSocket&& sock) noexcept = default;

    virtual ~HTcpSocket() = default;

    HTcpSocket& operator= (const HTcpSocket& ) = delete;

    HTcpSocket& operator= (HTcpSocket&&) noexcept = default;

public:
    HRET Init () override;


    HRET ShutDown (SYS_T how = SHUT_RDWR);    


    HRET Listen (HUN count) const;


    HRET Accept (HTcpSocket& sock, HAddr& ) const;    


    HRET ConnectWithTimeOut (const HAddr& addr, HN timeout) const;    


	HRET SetKeepAlive (HN keepAlive = SOCKET_KEEPALIVE_DEF, 
		HN keepIdle = SOCKET_KEEPIDLE_DEF, 
		HN keepInterval = SOCKET_KEEPINTERVAL_DEF, 
		HN keepCount = SOCKET_KEEPCOUNT_DEF) const;

    
    HRET SetupNodelay (HN val = 1) const;    


    SOCK_TYPE GetSocketType () const noexcept override {
	    return SOCK_TYPE::ST_TCP;
    }

};

class HDGramSock : public HSocket {
public:
    using base = HSocket;

protected:
    HDGramSock () noexcept : base () { }

    explicit HDGramSock (fd_t sock) noexcept : base (sock) { }

    HDGramSock(const HDGramSock& sock) = delete;

    HDGramSock(HDGramSock&& sock) noexcept = default;

    virtual ~HDGramSock() noexcept = default;

    HDGramSock& operator= (const HDGramSock& ) = delete;

    HDGramSock& operator= (HDGramSock&&) noexcept = default;

public:
    HOFF Recvfrom (HPTR buf, HSIZE size, SYS_T flags, HAddr& addr) const;

    HOFF Sendto (HCPTR cbuf, HSIZE size, SYS_T flags, const HAddr& addr) const;
};

class HUdpSock : public HDGramSock {
public:
    using base = HDGramSock;

public:
    HUdpSock () noexcept
	    : base () { }

    explicit HUdpSock (fd_t sock) noexcept
	    : base (sock) { }

    HUdpSock (const HUdpSock& sock) = delete;

    HUdpSock(HUdpSock&& sock) noexcept = default;

    virtual ~HUdpSock () noexcept { }

    HUdpSock& operator= (const HUdpSock& ) = delete;

    HUdpSock& operator= (HUdpSock&& sock) noexcept = default;

public:
    HRET Init () override;

    SOCK_TYPE GetSocketType () const noexcept override {
	    return SOCK_TYPE::ST_UDP;
    }

public:
    HOFF Read(HPTR buf, HSIZE size) const override;

    HOFF Write(HCPTR buf, HSIZE size) const override;  

    HRET SetupMulticastLoop() const;

    HRET SetupMulticastInterface(HCSTRR strAddr) const;

    HRET JoinBroadcast(HCSTRR strIP) const;

    HRET LeaveBroadcast(HCSTRR strIP) const;
    
};

class HUnixScoket : public HDGramSock {
public:
    using base = HDGramSock;
public:
    HUnixScoket () noexcept : base () { }

    explicit HUnixScoket (fd_t sock) noexcept : base (sock) { }

    HUnixScoket(const HUnixScoket& sock) = delete;

    HUnixScoket(HUnixScoket&& sock) noexcept = default;

    virtual ~HUnixScoket() = default;

    HUnixScoket& operator= (const HUnixScoket& ) = delete;

    HUnixScoket& operator= (HUnixScoket&&) noexcept = default;
    
public:
    HRET Init () override;

    SOCK_TYPE GetSocketType () const noexcept override { 
        return SOCK_TYPE::ST_UNIX;
    }

};


}

#endif //__H_HUICPP_SOCKET_H__



