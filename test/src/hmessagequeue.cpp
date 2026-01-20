

#include "hmessagequeue.h"

namespace HUICPP {

HMessageQueue::HMessageQueue() noexcept
	: base() {
}


HMessageQueue::HMessageQueue(key_type key, flag_type flag) noexcept
	: base (key, flag) {
}


HMessageQueue::~HMessageQueue() {

}


HRET HMessageQueue::Remove () {

	return control(IPC_RMID);

}


HRET HMessageQueue::Send (HCPTR ptr, size_type size, flag_type flags) {
    
    size_type send_size = size - sizeof(HLN);
      
	HASSERT_RETURN(msgsnd(GetId(), ptr,  send_size, flags) == 0, SYS_FAILED);
    
	HRETURN_OK;

}


HRET HMessageQueue::Recv (HPTR buf, HSIZER size, HLN lType, flag_type flags) {

    auto cb = msgrcv(GetId(), buf, size, lType, flags);
	HASSERT_RETURN(cb >= 0, SYS_FAILED);

	size = cb;

	HRETURN_OK;

}


HSIZE HMessageQueue::DataLength() const noexcept(false) {
    
    struct msqid_ds buf;

    stat_msqid(buf);

    return buf.__msg_cbytes;

}


HSIZE HMessageQueue::MsgCount() const noexcept(false) {

    struct msqid_ds buf;

    stat_msqid(buf);

    return buf.msg_qnum;

}


HSIZE HMessageQueue::MaxLength() const {

    struct msqid_ds buf;

    stat_msqid(buf);

    return buf.msg_qbytes;

}


HSIZE HMessageQueue::ClearMessage() const {

    HSIZE res = 0;
    HWORD_BUF buf = {0};
    while (msgrcv(GetId(), buf, HWORD_LEN, 0, IPC_NOWAIT | MSG_NOERROR) >= 0) {
        ++res;
    }
    
    return res;

}


HRET HMessageQueue::create () {

	if (HasInited()) {
        HRETURN_OK;
    }

	id_type msgid = msgget(GetKey(), GetFlag() | IPC_CREAT  | 600);

    if (msgid < 0) {
        if (errno == EEXIST) {
            HRETURN(SYS_FAILED);
        }

        HRETURN(SRC_ERR);
    }

    SetId(msgid);
    
    HRETURN_OK;

}


HRET HMessageQueue::open () {

	if(HasInited()) {
        HRETURN_OK;
    }

    id_type msgid = msgget(GetKey(), GetFlag());
    HASSERT_RETURN(msgid > 0, SYS_FAILED);

    SetId(msgid);

    HRETURN_OK;

}


HRET HMessageQueue::control (cmd_type cmd, struct msqid_ds* mds) const {

	HASSERT_RETURN(HasInited(), SRC_ERR);

    HASSERT_RETURN(msgctl(GetId(), cmd, mds) >= 0, SYS_FAILED);

    HRETURN_OK;

}


void HMessageQueue::stat_msqid(struct msqid_ds& buf) const {

    ZERO_MEM(&buf, sizeof(struct msqid_ds));

    HNOTOK_THROW(control(IPC_STAT, &buf));

}

}



