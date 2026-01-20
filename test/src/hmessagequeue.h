/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for message queue.
*/


#ifndef __H_HUICPP_MESSAGE_QUEUE_H__
#define __H_HUICPP_MESSAGE_QUEUE_H__

#include "huicpp.h"
#include "internal/ipcbase_in.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

namespace HUICPP {

using message_queue_base = hc_internal::ipc_basic<SYS_T>;

class HMessageQueue : public message_queue_base {
public:
	using base = message_queue_base;

public:
	HMessageQueue() noexcept;

	HMessageQueue(key_type key, flag_type flag = DEFAULT_IPC_FLAG) noexcept;

	~HMessageQueue();

public:
	virtual HRET Remove () override;

public:
	HRET Send (HCPTR ptr, size_type size, flag_type flags);

	HRET Recv (HPTR buf, HSIZER size, HLN lType, flag_type flags);

	HSIZE DataLength() const;

	HSIZE MsgCount() const;

	HSIZE MaxLength() const;

	HSIZE ClearMessage() const;

protected:
	virtual HRET create () override;

	virtual HRET open () override;

private:
	HRET control (cmd_type cmd, struct msqid_ds* mds = nullptr) const;

	void stat_msqid(struct msqid_ds& buf) const;

};

}



#endif //__H_HUICPP_MESSAGE_QUEUE_H__


