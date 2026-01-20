
#include "hsharememory.h"
#include "hstr.h"


namespace HUICPP {




HShareMemory::HShareMemory() noexcept
	: base(), m_size(0), m_shm_ptr(nullptr) {
	
}


HShareMemory::HShareMemory(key_type key, size_type shmsize, flag_type flag) noexcept 
    : base(key, flag), m_size(shmsize), m_shm_ptr(nullptr) {

}


HShareMemory::~HShareMemory() {

    HIGNORE_RETURN(dettach());

}


HRET HShareMemory::Remove() {

    HASSERT_RETURN(HasInited(), ERR_STATUS);

    HIGNORE_RETURN(dettach());

    HIGNORE_RETURN(control(IPC_RMID));

    SetInvalid();

    HRETURN_OK;
}


HRET HShareMemory::create() {

    if (HasInited()) {
        HRETURN_OK;
    }

    SYS_RET_T shmid = shmget(GetKey(), m_size, GetFlag() | IPC_CREAT | 600);
    
    if (shmid < 0) {

        if (errno == EEXIST) {
            HRETURN(SYS_FAILED);
        }

        HRETURN(SRC_ERR);
    }

    SetId(shmid);

    m_shm_ptr = static_cast<HUSZ>(shmat(GetId(), nullptr, 0));
    if (m_shm_ptr == reinterpret_cast<HUSZ>(-1)) {
        HIGNORE_RETURN(Remove());

        HRETURN(SYS_FAILED);
    }

    memset(m_shm_ptr, 0, m_size);
    
    HRETURN_OK;

}


HRET HShareMemory::open() {

    if(HasInited()) {
        HRETURN_OK;
    }

    SYS_RET_T shmid = shmget(GetKey(), m_size, GetFlag());
    HASSERT_RETURN(shmid > 0, SYS_FAILED);

    SetId(shmid);

    m_shm_ptr = static_cast<HUSZ>(shmat(GetId(), nullptr, 0));
    if (m_shm_ptr == reinterpret_cast<HUSZ>(-1)) {
        HIGNORE_RETURN(Remove());

        HRETURN(SYS_FAILED);
    }

    HRETURN_OK;

}


HRET HShareMemory::attach(HCPTR virtual_addr, flag_type flag) {

    HASSERT_RETURN(HasInited(), SRC_ERR);

    m_shm_ptr = static_cast<HUSZ>(shmat(GetId(), virtual_addr, flag));

    HASSERT_RETURN(m_shm_ptr != reinterpret_cast<HUSZ>(-1), SYS_FAILED);

	HRETURN_OK;

}

 
HRET HShareMemory::dettach() {

    HASSERT_RETURN(HasInited(), SRC_ERR);

    if (m_shm_ptr != nullptr) {

        if (shmdt(reinterpret_cast<HCPTR>(m_shm_ptr)) < 0) {

            HRETURN(SYS_FAILED);

        }

        m_shm_ptr = nullptr;

    }

    HRETURN_OK;

}


HRET HShareMemory::control(cmd_type cmd, struct shmid_ds * ds) {

    HASSERT_RETURN(HasInited(), SRC_ERR);

    HASSERT_RETURN(shmctl(GetId(), cmd, ds) >= 0, SYS_FAILED);

    HRETURN_OK;

}





}

