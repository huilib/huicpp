

#include "hsemaphore.h"
#include "htodoholder.h"
#include "hutil.h"

namespace HUICPP {

HSemaphore::HSemaphore() noexcept
    : base() {
}


HSemaphore::HSemaphore(key_type key, HN nsems, flag_type semflag) noexcept
    : base(key, semflag), m_nsems(nsems) {    
}
    #include "htodoholder.h"

HSemaphore::~HSemaphore() {
}


HRET HSemaphore::Remove(){

    HASSERT_RETURN(HasInited(), SRC_ERR);

    HASSERT_RETURN(semctl(GetId(), 0, IPC_RMID) >= 0, SYS_FAILED);

    HRETURN_OK;
    
}


HRET HSemaphore::Setall (HCUSNP init_array) {

    HUSNP array = const_cast<HUSNP>(init_array);

    if (array == nullptr) {
        array = new HUSN[m_nsems];
        for (int i = 0; i < m_nsems; ++i) {
            array[i] = 1;
        }
    }

    HSCOPE_EXIT {
        if (init_array == nullptr) {
            delete [] array;
        }
        array = nullptr;
    };

    union SVSemUn arg;
    arg.array = array;

    HNOTOK_RETURN(control(0, SETALL, arg));

    // Wait for the semaphore to be initialized.
    struct semid_ds seminfo;
    ZERO_MEM(&seminfo, sizeof(struct semid_ds));
    arg.buf = &seminfo;

    bool bTimeout = true;
    for (HN i = 0; i < MAX_TRIES; ++i) {

        HNOTOK_RETURN(control(0, IPC_STAT, arg));

        if (arg.buf->sem_otime != 0) {
            bTimeout = false;
            break;
        }

        HSleep(0, 100);
    }

    if (bTimeout) {
        HRETURN(TIME_OUT);
    }

    HRETURN_OK;

}


HRET HSemaphore::create () {

	if (HasInited()) {
        HRETURN_OK;
    }

    SYS_RET_T semid = semget(GetKey(), m_nsems, GetFlag() | IPC_CREAT | IPC_EXCL);
    
    HASSERT_RETURN(semid > 0 or errno != EEXIST, SRC_ERR);
    HASSERT_RETURN(semid > 0, SYS_FAILED);

    SetId(semid);
    
    HRETURN_OK;
}


HRET HSemaphore::open(){

    if(HasInited()) {
        HRETURN_OK;
    }

    SYS_RET_T semid = semget(GetKey(), 0, GetFlag());
    HASSERT_RETURN(semid > 0, SYS_FAILED);

    SetId(semid);

    HRETURN_OK;

}


HRET HSemaphore::Lock(HSN semnum , HSN sem_flg  ){

    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = -1;
    sops.sem_flg = sem_flg;

    return operation(&sops, 1);

}


HRET HSemaphore::Unlock(HSN semnum, HSN sem_flg  ){

    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = 1;
    sops.sem_flg = sem_flg;

    return operation(&sops, 1);
}


HRET HSemaphore::operation(struct sembuf *sops, HSIZE nsops){

    HASSERT_RETURN(semop(GetId(), sops, nsops) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET HSemaphore::control(HN semnum, cmd_type cmd, SVSemUn& arg) {

    HASSERT_RETURN(semctl(GetId(), semnum, cmd, arg) >= 0, SYS_FAILED);

    HRETURN_OK;

}


SemHolder::SemHolder(HSemaphore* sem, HSN index) 
	: m_sem(sem), m_index(index) {
	
	HIGNORE_RETURN(m_sem->Lock(m_index));
	
}

SemHolder::~SemHolder () {

	HIGNORE_RETURN(m_sem->Unlock (m_index));
	
}

}
