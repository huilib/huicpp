

#include "hlog.h"
#include "hprocesspool.h"
#include "hprocess.h"
#include <unistd.h>

namespace HUICPP {

namespace hc_internal_pool {

#define CHECK_INDEX(index) HASSERT_THROW(index >= 0 and index < m_nCount, INVL_PARA)

ProcessCntl::ProcessCntl(HN nCount, HN nCacheSize, HN nKey) noexcept 
    : m_nCount(nCount), m_nCacheSize(nCacheSize),
    m_mem(nKey, sizeof(cntl_info) * nCount + m_nCacheSize),
    m_pcntl_info(nullptr), m_cache_ptr(nullptr) {

}


ProcessCntl::~ProcessCntl() {

}


HRET ProcessCntl::Init() {

    HNOTOK_RETURN(m_mem.Init());

    HUSZ mem_ptr = m_mem.GetShmPtr();
    m_pcntl_info = reinterpret_cast<cntl_info_ptr>(mem_ptr);
    ZERO_MEM(m_pcntl_info,sizeof(cntl_info) * m_nCount);

    for (HN i = 0; i < m_nCount; ++i) {
        ResetInfo(i);
    }

    m_cache_ptr = reinterpret_cast<HPTR>(mem_ptr + sizeof(cntl_info) * m_nCount);

    HRETURN_OK;
}


RET_T ProcessCntl::Uninit (bool isSon) {

    if (isSon) {

        HNOTOK_RETURN(m_mem.dettach());

    } else {

        HNOTOK_RETURN(m_mem.Remove());

    }

    m_pcntl_info = nullptr;
    m_cache_ptr = nullptr;

    HRETURN_OK;

}


void ProcessCntl::ResetInfo(HN index) {
    CHECK_INDEX(index);
    m_pcntl_info[index].at = m_pcntl_info[index].startat = m_pcntl_info[index].pid = 0;
    m_pcntl_info[index].status = HTO_INT(status_t::CS_UNUSED);
}


void ProcessCntl::SetStartInfo (HN index, pid_t pid) {
    CHECK_INDEX(index);
    m_pcntl_info[index].startat = time(nullptr);
    m_pcntl_info[index].at = 0;
    m_pcntl_info[index].pid = pid;
    m_pcntl_info[index].status = HTO_INT(status_t::CS_IDLE);
}


void ProcessCntl::TouchInfo (HN index) {
    set_info_as_status(index, status_t::CS_BUSY);
}


void ProcessCntl::SetIdleInfo(HN index) {
    set_info_as_status(index, status_t::CS_IDLE);
}

        
void ProcessCntl::UnuseInfo (HN index) {
    set_info_as_status(index, status_t::CS_UNUSED);
}


time_t ProcessCntl::GetStartAt(HN index) const {
    CHECK_INDEX(index);
    return m_pcntl_info[index].startat;
}


time_t ProcessCntl::GetLastAccessTime (HN index) const {
    CHECK_INDEX(index);
    return m_pcntl_info[index].at;
}


pid_t ProcessCntl::GetPid (HN index) const {
    CHECK_INDEX(index);
    return m_pcntl_info[index].pid;
}


ProcessCntl::status_t ProcessCntl::GetStatus (HN index) const {
    CHECK_INDEX(index);
    return static_cast<status_t>(m_pcntl_info[index].status);
}


void ProcessCntl::set_info_as_status(HN index, status_t status) {
    CHECK_INDEX(index);
    m_pcntl_info[index].at = time(nullptr);
    m_pcntl_info[index].pid = GetPid(index);
    m_pcntl_info[index].status = HTO_INT(status);
}


#undef CHECK_INDEX

}

HProcessPool::HProcessPool(HN nChildCount, HN nCacheSize, HN nKey) noexcept 
    : base(nChildCount), m_bInited(false), 
    m_cntl(nChildCount, nCacheSize, nKey),
    m_index(PARENT_DEFAULT_INDEX) {
}


HProcessPool::~HProcessPool() {

    if (HasInited()) {
        if (not IsSon()) {
            for (HN i = 0; i < GetChildCount(); ++i) {
                kill_child(i);
            }
        }
        m_cntl.Uninit(IsSon());
    }
    
}


HRET HProcessPool::Init () {

    SLOG_NORMAL("HProcessPool init count[%d]", GetChildCount());

    HNOTOK_MSG_RETURN(m_cntl.Init(), "control init failed");

    for (HN i = 0; i < GetChildCount(); ++i) {
        if (IsSon()) {
            break;
        }
        create_child(i);
    }

    m_bInited = true;
    HRETURN_OK;
}


void HProcessPool::ManagePool () {
    if (IsSon()) {
        son_touch();
    } else {
        parent_touch();
    }
}


void HProcessPool::prepare_end_son(HN) {
}


void HProcessPool::create_child (HN index) {

    SLOG_NORMAL("create son process [%d]", index);

    pid_t pid = fork();
    HASSERT_MSG_THROW(pid >= 0, "fork failed", SYS_FAILED);

    if (pid == 0) {
        // child 
        m_index = index;
        childProcessRun();
    } else {
        // parent
        m_cntl.SetStartInfo(index, pid);
    }

}


void HProcessPool::kill_child(HN index) {

    // call prepare_end_son first, make customer can clear the resource before kill son process.
    prepare_end_son(index);

    pid_t pid = m_cntl.GetPid(index);
    HIGNORE_RETURN(HProcess::KillProcess(pid));
    sleep(AFTER_SIGTERM_ST);
    HProcess::KillProcessAnyway(pid);
    m_cntl.UnuseInfo(index);
}


void HProcessPool::parent_touch () {

    time_t nt = time(nullptr);

    for (HN i = 0; i < GetChildCount(); ++i) {

        pid_t pid = m_cntl.GetPid(i);

        if (not HProcess::ProcessExists(pid)) {

            SLOG_WARNING("son process index[%d] pid[%d] is not EXISTS", i, pid);

            kill_child(i);

            create_child(i);

            if (IsSon()) {
                break;
            }

        }

        time_t at = m_cntl.GetLastAccessTime(i);
        if (at == 0) {
            // if the process is just start. we check the start time.
            at = m_cntl.GetStartAt(i);
        }

        if (abs(nt - at) > MAX_HEAR_TIME) {

            SLOG_WARNING("son process index[%d] acesstime[%d] nowtime[%d] will be restart",
                i, at, nt);

            kill_child(i);

            create_child(i);

            if (IsSon()) {
                break;
            }
        }

    }

}


void HProcessPool::son_touch () {
    m_cntl.TouchInfo(m_index);
}



}

