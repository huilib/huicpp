/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for Linux semaphore.
*/

#ifndef __H_HUICPP_SEMAPHORE_H__
#define __H_HUICPP_SEMAPHORE_H__

#include "huicpp.h"
#include "internal/ipcbase_in.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


namespace HUICPP {

union SVSemUn {
    int val;                  /* value for SETVAL */
    struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;    /* array for GETALL, SETALL */
                              /* Linux specific part: */
    struct seminfo *__buf;    /* buffer for IPC_INFO */
};

using semaphore_base = hc_internal::ipc_basic<SYS_T>;

class HSemaphore : public semaphore_base {
public:
    using base = semaphore_base;
    static constexpr const HN MAX_TRIES = 5;

public:
    HSemaphore() noexcept;

    HSemaphore(key_type key, HN nsems, flag_type semflag = DEFAULT_IPC_FLAG) noexcept;

    virtual ~ HSemaphore();

public:
    virtual HRET Remove() override;

    HRET Setall (HCUSNP init_array = nullptr);

    HRET Lock(HSN semnum = 0, HSN sem_flg = SEM_UNDO);

    HRET Unlock(HSN semnum = 0, HSN sem_flg = SEM_UNDO);

protected:
    virtual HRET create() override;

    virtual HRET open() override;

private:
    HRET operation(struct sembuf *sops, HSIZE nsops);

    HRET control(HN semnum, cmd_type cmd, SVSemUn& arg);


private:
    HN m_nsems;
};

class SemHolder {
public:
	SemHolder (HSemaphore* sem, HSN index);

	~SemHolder ();

private:
	HSemaphore* m_sem;
	HSN m_index;
};

}

#endif  //__H_HUICPP_SEMAPHORE_H__

