
#include "hsocket.h"
#include "htodoholder.h"
#include "htime.h"
#include <sys/select.h>

#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <functional>

namespace HUICPP  {


HRET HSocket::Bind (const HAddr& addr) const  {

	return addr.Bind(*this);

}


HRET HSocket::Connect (const HAddr& addr) const {
	return addr.Connect(*this);
}


HRET HSocket::GetAddrInfo (HAddr& addr) const {
	RETURN_SOCK_STATUS;
	return addr.GetFromSocket(*this);
}


HRET HSocket::GetAddr4Info(HSTRR strIp, HNR iPort) const {
	HIp4Addr addr;
	HNOTOK_RETURN(GetAddrInfo(addr));
    return addr.GetAddrInfo(strIp, iPort);
}


HSTR HSocket::ToString () const{
	HSTR strIp;
	HN iPort = 0;
	HNOTOK_THROW(GetAddr4Info(strIp, iPort));
	return HStr::Format("%s:%d", strIp.c_str(), iPort);
}


HOFF HSocket::Pread(HPTR, HSIZE, HOFF) const {
	printf("FUNCTION[HOFF HSocket::Pread(HPTR, HSIZE, HOFF) const] UNSUPPORTED!\n");
	return 0;
}


HOFF HSocket::Pwrite(HCPTR, HSIZE, HOFF) const {
	printf("FUNCTION[HOFF HSocket::Pwrite(HPTR, HSIZE, HOFF) const] UNSUPPORTED!\n");
	return 0;
}


HOFF HSocket::WriteAll (HCPTR buf, HSIZE nbyte, HN timeout) const noexcept(false){

	SOCK_FD_CHECK;

	HCSZ pos = static_cast<HCSZ>(buf);

	HSIZE sent_bytes = 0; 
	HOFF this_send = 0;
	
	struct timeval tval = HMicroTime(timeout).GetTimeVal();

	while (sent_bytes < nbyte) {

		if (timeout > 0) {

			fd_set write_fds;
			FD_ZERO(&write_fds);
			FD_SET(Fd(), &write_fds);
	
			SYS_RET_T cb = select(Fd() +1, nullptr, &write_fds, nullptr, &tval);
			if (cb == 0) {
				THROW_NET_EXCEPTION("write all time out");
			} else if (cb < 0) {
				THROW_NET_EXCEPTION("select failed");
			}
		}
		
		this_send = Write(pos, nbyte - sent_bytes);

		if (this_send <= 0) {
			return this_send;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;
}


HOFF HSocket::ReadWithTimeOut (HPTR buf, HSIZE nbyte, HN timeout) const noexcept(false){
	
	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(Fd(), &read_fds);

		struct timeval tval = HMicroTime(timeout).GetTimeVal();

		SYS_RET_T cb = select(Fd() +1, &read_fds, nullptr, nullptr, &tval);
		if (cb == 0) {
			THROW_NET_EXCEPTION("read all time out");
		} else if (cb < 0) {
			THROW_NET_EXCEPTION("select failed");
		}

	}

    return Read(buf, nbyte);
	
}


HOFF HSocket::ReadAllWithTimeout (HPTR buf, HSIZE nbyte, HN timeout) const {

	HOFF cb = 0, ret = 0;
	HSZ pos = static_cast<HSZ>(buf);
	HSIZE temp_nbyte = nbyte;

	do {

		try {

			cb = ReadWithTimeOut(pos, temp_nbyte, timeout);

			if (cb > 0) {
				ret += cb;
				pos += cb;
				temp_nbyte -= cb;
			} 
			
		} catch (...) {
			break;
		}

	} while(cb > 0);

	return ret;

}


HOFF HSocket::Send (HCPTR buf, HSIZE len, SYS_T flags) const {	
	return ::send(Fd(), buf, len, flags);
}


HOFF HSocket::SendAll (HCPTR buf, HSIZE len, SYS_T flags, HN timeout) const noexcept(false) {

	SOCK_FD_CHECK;

	HCSZ pos = static_cast<HCSZ>(buf);

	HSIZE sent_bytes = 0; 
	HOFF this_send = 0;
	
	struct timeval tval = HMicroTime(timeout).GetTimeVal();

	while (sent_bytes < len) {

		if (timeout > 0) {

			fd_set write_fds;
			FD_ZERO(&write_fds);
			FD_SET(Fd(), &write_fds);
	
			SYS_RET_T cb = select(Fd() +1, nullptr, &write_fds, nullptr, &tval);
			if (cb == 0) {
				THROW_NET_EXCEPTION("write all time out");
			} else if (cb < 0) {
				THROW_NET_EXCEPTION("select failed");
			}
		}

		this_send = ::send(Fd(), pos, len - sent_bytes, flags);

		if (this_send <= 0) {
			return this_send;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;

}


HOFF HSocket::Recv (HPTR buf, HSIZE len, SYS_T flags) const {
	return recv(Fd(), buf, len, flags);
}


HOFF HSocket::RecvWithTimeOut (HPTR buf, 
	HSIZE len, SYS_T flags , HN timeout ) const noexcept(false) {

	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(Fd(), &read_fds);

		struct timeval tval = HMicroTime(timeout).GetTimeVal();

		SYS_RET_T cb = select(Fd() +1, &read_fds, nullptr, nullptr, &tval);
		if (cb == 0) {
			THROW_NET_EXCEPTION("read all time out");
		} else if (cb < 0) {
			THROW_NET_EXCEPTION("select failed");
		}
		
	}

	return ::recv(Fd(), buf, len, flags);
}


HRET HSocket::SetSockOpt (SYS_T optName, HCPTR optVal,
		     socklen_t optLen, SYS_T level ) const {

	SYS_RET_T cb = setsockopt(Fd(), level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HRET HSocket::GetSockOpt (SYS_T optName, HPTR optVal, socklen_t* optLen, SYS_T level) const {

	SYS_RET_T cb = getsockopt(Fd(), level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}


HRET HSocket::SetReuseAddr () const {
	int rea = 1;
	return SetSockOpt(SO_REUSEADDR, 
		static_cast<void*>(&rea), sizeof(rea), SOL_SOCKET);
}


HRET HSocket::SetReusePort () const {

	int val = 1;
	return SetSockOpt(SO_REUSEPORT, 
		static_cast<void*>(&val), sizeof(val), SOL_SOCKET);

}


HRET HSocket::GetSendBufferLength(HN & buf_len) const {

	buf_len = 0;
	socklen_t len = sizeof(buf_len);
	return GetSockOpt(SO_SNDBUF, static_cast<HPTR>(&buf_len), &len, SOL_SOCKET);

}


HRET HSocket::SetupSendBufLength(HN buf_len) const {

	return SetSockOpt(SO_SNDBUF, static_cast<HCPTR>(&buf_len), sizeof(buf_len), SOL_SOCKET);

}


HRET HSocket::GetRecvBufferLength(HN & buf_len) const {

	buf_len = 0;
	socklen_t len = sizeof(buf_len);
	return GetSockOpt(SO_RCVBUF, static_cast<HPTR>(&buf_len), &len, SOL_SOCKET);

}
    
    
HRET HSocket::SetupRecvBufLength(HN buf_len) const {

	return SetSockOpt(SO_RCVBUF, static_cast<HCPTR>(&buf_len), sizeof(buf_len), SOL_SOCKET);

}


HRET HTcpSocket::Init ()  {
	
	// close if socket is established.
	Close();

	fd_t sock = socket(AF_INET, SOCK_STREAM, 0);
	HASSERT_RETURN(sock > 0, SYS_FAILED);
	SetFd(sock);

	HRETURN_OK;

}


HRET HTcpSocket::ShutDown (SYS_T how) {

	SYS_RET_T cb = ::shutdown (Fd(), how);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}


HRET HTcpSocket::Listen (HUN _count) const{

	SYS_RET_T cb = ::listen(Fd(), _count);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}


HRET HTcpSocket::Accept (HTcpSocket& sock, HAddr& addr)  const{

	return addr.Accept(*this, sock);
	
}


HRET HTcpSocket::ConnectWithTimeOut (const HAddr& addr, HN timeout) const {

	RETURN_SOCK_STATUS;

	if (timeout <= 0) { timeout = SOCKET_DEFAULT_TIMEOUT; }

	// setup SetNonblocked holder; 
	bool bHasSet = false;
	if (not IsNonblocked()) {
		bHasSet = true;
		HIGNORE_RETURN(SetNonblocked());
	}

	HSCOPE_EXIT { 
		if (bHasSet) {
			HIGNORE_RETURN(this->SetNonblocked(false));
		}
	};

	struct timeval tval = HMicroTime(timeout).GetTimeVal();

	// try to connect
	while(NOT_OK(Connect(addr))) {

		// errno == EINPROGRESS

		fd_set wfds;		
		FD_ZERO(&wfds);
		FD_SET(Fd(), &wfds);

		HN cb = 0;

		while((cb = select(Fd() + 1, nullptr, &wfds, nullptr, &tval)) < 0) {		
			HASSERT_RETURN(errno == EINTR, SYS_FAILED);
			continue;
		}

		if (cb == 0) {
			HRETURN(TIME_OUT);
		} else {
			
			if (errno == EHOSTUNREACH) {
				// No route to host
				HRETURN(NETWORK_ERROR);
			} else {
				// almost finished...
				SYS_T iError = -1;
				HSIZE slen = sizeof(iError);
				(void)getsockopt(Fd(), SOL_SOCKET, SO_ERROR, &iError, (socklen_t*)&slen);
				HASSERT_RETURN(iError == 0, NETWORK_ERROR);
			}
			
		}
		
	}

	HRETURN_OK;

}



/**
	keepAlive: trun on keepalive
	keepIdle: try to network plumbing if no data transafe in 600's
	keepInterval: timeval 
	keepCount: times for network plumbing
**/
HRET HTcpSocket::SetKeepAlive (HN keepAlive, HN keepIdle, HN keepInterval, HN keepCount) const {

	RETURN_SOCK_STATUS;

	if (keepAlive >= 0) {
		HNOTOK_RETURN(SetSockOpt(SO_KEEPALIVE, 
			static_cast<HCPTR>(&keepAlive), sizeof(keepAlive), SOL_SOCKET));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPIDLE, 
			static_cast<HCPTR>(&keepIdle), sizeof(keepIdle), SOL_TCP));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPINTVL, 
			static_cast<HCPTR>(&keepInterval), sizeof(keepInterval), SOL_TCP));
	}

	if (keepAlive >= 0) {
    	HNOTOK_RETURN(SetSockOpt(TCP_KEEPCNT, 
			static_cast<HCPTR>(&keepCount), sizeof(keepCount), SOL_TCP));
	}

	HRETURN_OK;
}


HRET HTcpSocket::SetupNodelay (HN val) const {

	return SetSockOpt(TCP_NODELAY, static_cast<HCPTR>(&val), sizeof(val), IPPROTO_TCP);

}


HOFF HDGramSock::Recvfrom (HPTR buf, HSIZE size, SYS_T flags, HAddr& addr) const {

	return addr.Recvfrom(*this, buf, size, flags);

}


HOFF HDGramSock::Sendto (HCPTR cbuf, HSIZE size, SYS_T flags, const HAddr& addr) const {

	return addr.Sendto(*this, cbuf, size, flags);

}


HRET HUdpSock::Init() {

	// close if socket is established.
	Close();

	fd_t sock = socket(AF_INET, SOCK_DGRAM, 0);
	HASSERT_RETURN(sock > 0, SYS_FAILED);
	SetFd(sock);

	HRETURN_OK;

}


HOFF HUdpSock::Read(HPTR buf, HSIZE size) const {
	return Recv(buf, size);
}


HOFF HUdpSock::Write(HCPTR buf, HSIZE size) const {
	return Send(buf, size);
}


HRET HUdpSock::SetupMulticastLoop() const {

	const HUCH val = 1;
	return SetSockOpt(IP_MULTICAST_LOOP, &val, sizeof(val), IPPROTO_IP);

}


HRET HUdpSock::SetupMulticastInterface(HCSTRR strAddr) const {

	struct in_addr addr;

	HASSERT_RETURN(inet_pton(AF_INET, strAddr.c_str(), &addr) == 1, NETWORK_ERROR);

	return SetSockOpt(IP_MULTICAST_IF, &addr, sizeof(addr), IPPROTO_IP);

}


HRET HUdpSock::JoinBroadcast(HCSTRR strIP) const {

	struct ip_mreq imr4;

	imr4.imr_multiaddr.s_addr = inet_addr(strIP.c_str());
	imr4.imr_interface.s_addr = INADDR_ANY;

	return SetSockOpt(IP_ADD_MEMBERSHIP, &imr4, sizeof(imr4), IPPROTO_IP);

}


HRET HUdpSock::LeaveBroadcast(HCSTRR strIP) const {

	struct ip_mreq imr4;

	imr4.imr_multiaddr.s_addr = inet_addr(strIP.c_str());
	imr4.imr_interface.s_addr = INADDR_ANY;

	return SetSockOpt(IP_DROP_MEMBERSHIP, &imr4, sizeof(imr4), IPPROTO_IP);

}


HRET HUnixScoket::Init () {
	// close if socket is established.
	Close();

	fd_t sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	HASSERT_RETURN(sock > 0, SYS_FAILED);
	SetFd(sock);

	HRETURN_OK;
}


}


