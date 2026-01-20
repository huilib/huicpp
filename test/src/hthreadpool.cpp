
#include "hthreadpool.h"
#include <hlog.h>

namespace HUICPP {

HThreadPool::HThreadPool(HN uChildCount) noexcept 
    : base(uChildCount), m_pinfos(nullptr), m_mutex() {
    

}


HThreadPool::~ HThreadPool () {

    free_info();

}


HRET HThreadPool::Init () {

    if (GetChildCount() == 0) {
        HRETURN_OK;
    }

    create_info();

    thread_arg * parr = static_cast<thread_arg*>(malloc(
        sizeof(thread_arg) * GetChildCount()));
    ZERO_MEM(parr, sizeof(thread_arg) * GetChildCount());

    HThread thread;
    for (HN i = 0; i < GetChildCount(); ++i) {

        parr[i].m_index = i;
        parr[i].m_pool = this;

        HNOTOK_THROW(thread.Create(thread_init_run, &(parr[i])));

        s_son_lock.WaitSonRun(s_uChildCreated);

        m_pinfos[i].m_thread = std::move(thread);
        m_pinfos[i].m_cs = ChildStatus::CS_IDLE;

    }

    free(parr);
    HRETURN_OK;

}


HN HThreadPool::GetThreadIndex() const {

    thread_arg* thread_para = reinterpret_cast<thread_arg*>(
        CreateThreadSpecific(sizeof(thread_arg)));

    return thread_para->m_index;

}


void HThreadPool::childThreadExit() {

    // thread id.
    HN thread_id = GetThreadIndex();

    set_child_status(ChildStatus::CS_UNUSED);

    beforeThreadExit(thread_id);

    pthread_exit(nullptr);

}


void HThreadPool::JoinAll() {

    // release threads first.
    for (HN i = 0; i < GetChildCount(); ++i) {

        HIGNORE_RETURN(m_pinfos[i].m_thread.Join(nullptr));

    }

}


void HThreadPool::create_info() {

    // if no child, we do nothing.
    if (GetChildCount() == 0) {
        return ;
    }


    m_pinfos = new child_info[GetChildCount()];
    CHECK_NEWPOINT(m_pinfos);

    for (HN i = 0; i < GetChildCount(); ++i) {

        m_pinfos[i].m_cs = ChildStatus::CS_UNUSED;

    }

}


void HThreadPool::free_info () {

    // free threads first.
    for (HN i = 0; i < GetChildCount(); ++i) {

        if (m_pinfos[i].m_cs == ChildStatus::CS_UNUSED) {
            continue;
        }

        HIGNORE_RETURN(m_pinfos[i].m_thread.Cancel());

        HIGNORE_RETURN(m_pinfos[i].m_thread.Join(nullptr));

    }

    HDEL_ARR(m_pinfos);

}


void HThreadPool::set_child_status(ChildStatus status) {

    thread_arg* thread_para = reinterpret_cast<thread_arg*>(
        CreateThreadSpecific(sizeof(thread_arg)));
    HASSERT_MSG_THROW(thread_para != nullptr and thread_para->m_index < GetChildCount(), 
        "thread para is invalid", SRC_ERR);
    m_pinfos[thread_para->m_index].m_cs = status;

}


void* HThreadPool::thread_init_run(void* argv) {

    thread_arg* arg = static_cast<thread_arg*>(argv);
    HThreadPool* pool = arg->m_pool;
    HUN index = arg->m_index;

    HASSERT_THROW(pool != nullptr, INVL_PARA);

    s_son_lock.SonRun(s_uChildCreated);

    thread_arg* thread_para = reinterpret_cast<thread_arg*>(
        CreateThreadSpecific(sizeof(thread_arg)));
    thread_para->m_index = index;
    thread_para->m_pool = pool;

    // 设置线程状态为busy
    pool->set_child_status(ChildStatus::CS_BUSY);

    pool->childThreadRun();

    return nullptr;

}


HWaitSonLock HThreadPool::s_son_lock;
HThreadKey HThreadPool::s_thread_key;
HThreadOnce HThreadPool::s_thread_once;
HN HThreadPool::s_uChildCreated = 0;


void HThreadPool::ThreadSpecifiOnce () {

    HIGNORE_RETURN(s_thread_key.Create(ThreadSpecifiDestructor));

}


void HThreadPool::ThreadSpecifiDestructor(void* ptr) {

    HSZ pt = static_cast<HSZ>(ptr);
    HDEL_ARR(pt);

}


HSZ HThreadPool::CreateThreadSpecific(HUN len) {

    HSZ res = nullptr;
    s_thread_once.Once(ThreadSpecifiOnce);

    if ((res = static_cast<HSZ>(s_thread_key.Get())) == nullptr) {

        res = new HCH[len];
        HNOTOK_THROW(s_thread_key.Set(res));

        memset(res, 0, len);

    }

    return res;

}


}