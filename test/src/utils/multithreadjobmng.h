

#ifndef __H_UTILS_MULTITHREAD_JOB_MNG_H__
#define __H_UTILS_MULTITHREAD_JOB_MNG_H__

#pragma once

#include "../huicpp.h"
#include "../hservlet.h"
#include "../hthreadpool.h"
#include "jobpool.h"

namespace HUICPP {

/*
    多线程任务管理类.
    需要被继承, 其中protected api 'initJobs' 需要被重写.
*/
class MultiThreadJobMng : public HThreadPool, public JobPool {
public:
    using base_threadpool_class = HThreadPool;
    using base_jobpool_class = JobPool;

public:
    MultiThreadJobMng(HN nWork = 2);

    /*
        initJobs是纯虚接口, 因此析构函数定义为virtual.
    */
    virtual ~ MultiThreadJobMng();

public:
    /*
        主要的初始化接口.
        初始化任务, 工作线程.
    */
    HRET InitJobSummer();

    /*
        所有任务是否被处理完?
    */
    bool Finish();

protected:

    /*
        线程工作接口. 获取任务进行工作.
    */
    void childThreadRun () override;

    /*
        需要知道线程退出. Finish接口需要知道所有线程是否都已结束;
    */
    void beforeThreadExit(HN) override;

    /*
        初始化任务的接口, 纯虚接口可以被override.
    */
    virtual HRET initJobs() = 0;

private:
    HN m_nWorking;
};

}

#endif // __H_UTILS_MULTITHREAD_JOB_MNG_H__

