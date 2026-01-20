

#include "jobpool.h"


namespace HUICPP {

/*
    JobPool 构造函数, 它将初始化Lock和jobs container.
    因为HMutex构造函数是except attribution, 所以JobPool 构造函数不能定义为noexcep.
*/
JobPool::JobPool()  
    : m_mutex(), m_jobs() {



}


JobPool::~ JobPool() {

    // 调用clear 清除所有任务. 
    // m_jobs存放的是任务指针, JobPool析构时需要确保任务指针的内存不能泄漏.
    clear();

}



/*
    向任务池添加任务, 一般由servlet类调用;
    ptr 定义为const-pointer, 迎合OPP接口设计.
*/
void JobPool::PushJob(job_cptr ptr) {

    /*
        向m_jobs添加任务前, 需要获取锁.
    */
    HSCOPE_MUTEX(m_mutex);

    // 转换为non-const 指针.
    m_jobs.push(const_cast<job_ptr>(ptr));

}


/*
    获取任务. 由工作线程调用.
*/
JobPool::job_ptr JobPool::GetJob() {

    /*
        m_jobs出对列前, 需要获取锁.
    */
    HSCOPE_MUTEX(m_mutex);

    if (m_jobs.empty()) {
        /*
            如果任务队列没有任务, 返回nullptr. 工作线程停止.
        */
        return nullptr;
    }

    job_ptr ptr = m_jobs.front();
    m_jobs.pop();

    return ptr;

}


/*
    清除所有任务. 
    定义该接口只在类内部使用, 所以定义为protected attribution.
*/
void JobPool::clear() {

    /*
        清除m_jobs里的数据前, 需要获取锁.
    */
    HSCOPE_MUTEX(m_mutex);

    /*
        清除任务队列里的数据, 并释放内存.
    */
    while (not m_jobs.empty()) {

        job_ptr ptr = m_jobs.front();
        HDELP(ptr);
        m_jobs.pop();

    }

}


/*
    用于检测任务是否处理完
*/
bool JobPool::allFinished() const {

    HSCOPE_MUTEX(m_mutex);

    return m_jobs.empty();

}


}
