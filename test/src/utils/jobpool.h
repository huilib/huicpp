

#ifndef __H_UTILS_JOBPOOL_H__
#define __H_UTILS_JOBPOOL_H__

#pragma once

#include "../huicpp.h"
#include "../hservlet.h"
#include <queue>
#include "jobbase.h"
#include "../hmutex.h"

namespace HUICPP {

/*
    A job pool with lock.
*/
class JobPool {
public:
    using job_t = JobBase;
    using job_ptr = job_t*;
    using job_cptr = const JobBase*;

public:
    JobPool();

    ~ JobPool();

    /*
        JobPool need not to initilize.
        Servlet will push job to the pool by call `PushJob`.
    */
    // HRET Init();

    /*
        添加任务.
    */
    void PushJob(job_cptr ptr);

    /*
        获取任务. 由工作线程调用.
        调用者需要释放任务的内存.
    */
    job_ptr GetJob();

protected:
    /*
        清除所有任务.
    */
    void clear();

    /*
        任务是否都已被处理完? 
    */
    bool allFinished() const;

private:    
    /*
        JobPool need a lock. 
        m_mutex will protect m_jobs in multi-threads environment.
    */
    mutable HMutex m_mutex;

    /*
        A FIFO job queue. 
        m_jobs hold all jobs.
    */
    std::queue<job_ptr> m_jobs;    
};  

}

#endif // __H_UTILS_JOBPOOL_H__


