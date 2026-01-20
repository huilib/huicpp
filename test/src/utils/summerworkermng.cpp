

#include "summerworkermng.h"
#include <memory>


namespace HUICPP {

JobQueue::JobQueue()
    : m_locker(), m_queue() {

}


JobQueue::~JobQueue() {


}


void JobQueue::PushJob(job_ptr_t ptr) {

    {
        // 增加数据时, 先锁住队列
        HSCOPE_MUTEX(m_locker);
        m_queue.push(ptr);
    }

    // 广播信号
    HNOTOK_THROW(m_locker.Signal());

}


JobQueue::job_ptr_t JobQueue::PopJob() {

    HSCOPE_MUTEX(m_locker);

    while(m_queue.empty()) {

        HNOTOK_THROW(m_locker.Wait());

    }

    // 安全判断, 防止平台差异性
    if (m_queue.empty()) {
        return nullptr;
    }

    job_ptr_t res = m_queue.front();

    // 删除第一个
    m_queue.pop();

    return res;

}


void JobQueue::PopJobs(jobs_t& jobs) {

    HSCOPE_MUTEX(m_locker);

    while(m_queue.empty()) {

        HNOTOK_THROW(m_locker.Wait());

    }

    while (not m_queue.empty()) {

        // 在模板中, 还是需要用move
        jobs.push_back(m_queue.front());

        m_queue.pop();

    }

}


bool JobQueue::HasData() const {

    HSCOPE_MUTEX(m_locker);

    return m_queue.empty();

}



SummerWorkerMng::SummerWorkerMng(HN nWork)
    : thread_pool_class(nWork), queue_base_class() {

}


SummerWorkerMng::~ SummerWorkerMng() {

}


void SummerWorkerMng::childThreadRun () {

    while (true) {

        std::shared_ptr<job_t> job(PopJob());

        if (job == nullptr) {

            // 没有任务时, 终止.
            childThreadExit();

            printf("!!!!!!SHOULD NOT PRINT!!!!!!\n");
            return ;

        }

        job->DoJob();

    }


}


void SummerWorkerMng::ExitWork() {

    HN cc = GetChildCount();
    for (HN i = 0; i < cc; ++i) {

        ExitThreadJob* job = new ExitThreadJob();
        PushJob(job);

    }

    JoinAll();

}


}
