

#ifndef __H_UTILS_SUMMERWORKERMNG_H__
#define __H_UTILS_SUMMERWORKERMNG_H__

#include "../hthreadpool.h"
#include "jobbase.h"

#include <queue>

namespace HUICPP {

/*
    FIFO 任务队列.
*/
class JobQueue {
public:
    using job_t = JobBase;
    using job_ptr_t = job_t*;
    using job_cptr = const job_t*;
    using job_queue_t = std::queue<job_ptr_t>;
    using jobs_t = std::vector<job_ptr_t>;

public:
    JobQueue();

    ~ JobQueue();

public:
    /*
        添加任务
    */
    void PushJob(job_ptr_t ptr);

    /*
        取任务.
    */
    job_ptr_t PopJob();

    /*
        取所有任务.
    */
    void PopJobs(jobs_t& jobs);

    /*
        检查任务.
    */
    bool HasData() const;

private:
    mutable HConLock m_locker;
    job_queue_t m_queue;
};


class SummerWorkerMng : public HThreadPool, public JobQueue {
public:
    using thread_pool_class = HThreadPool;
    using queue_base_class = JobQueue;

public:
    SummerWorkerMng(HN nWork = 2);

    /*
        虚接口, 此类会被继承.
    */
    virtual ~ SummerWorkerMng();

public:
    /*
        线程工作接口. 获取任务进行工作.
    */
    void childThreadRun () override;   

    void ExitWork();
};

}

#endif // __H_UTILS_SUMMERWORKERMNG_H__

