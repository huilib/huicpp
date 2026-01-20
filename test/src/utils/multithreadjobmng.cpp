

#include "multithreadjobmng.h"
#include <memory>

namespace HUICPP {


MultiThreadJobMng::MultiThreadJobMng(HN nWork)
    : base_threadpool_class(nWork),
    base_jobpool_class(),
    m_nWorking(nWork) {

}


MultiThreadJobMng::~ MultiThreadJobMng() {

}


/*
    主要的初始化接口.
    初始化任务, 工作线程.
*/
HRET MultiThreadJobMng::InitJobSummer() {

    // 初始化任务池.
    HNOTOK_RETURN(initJobs());

    // 初始化servlet.
    HNOTOK_RETURN(base_threadpool_class::Init());

    HRETURN_OK;

}


/*
    所有任务是否被处理完?
    处理完的条件:
        > 任务队列是空;
        > 没有工作线程都退出;
    NOTE: 只有在InitJobSummer成功后调用才有意义.
*/
bool MultiThreadJobMng::Finish() {

    return m_nWorking == 0 and allFinished();

}


/*
    线程工作接口. 获取任务进行工作.
*/
void MultiThreadJobMng::childThreadRun () {

    while (true) {

        std::shared_ptr<base_jobpool_class::job_t> job(GetJob());

        if (job == nullptr) {

            // 没有任务时, 终止.
            childThreadExit();

            printf("!!!!!!SHOULD NOT PRINT!!!!!!\n");
            return ;

        }

        job->DoJob();

    }

}


void MultiThreadJobMng::beforeThreadExit(HN) {

    m_nWorking--;

}


}


