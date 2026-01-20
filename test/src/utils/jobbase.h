

#ifndef __H_UTILS_JOBBASE_H__
#define __H_UTILS_JOBBASE_H__

#pragma once

#include "../huicpp.h"

namespace HUICPP {

/*
    TODO:
        可以设计一种专门的线程退出任务, 当工作线程接受到这种任务时, 直接退出线程.
*/

/*
    Tha abstraction job base class.
*/
class JobBase {
public:
    /*
        JobBase 基类构造未初始化any source and call any api that will throw exception.
        Declare the contruction function as noexcept.
    */
    JobBase(HCSTRR strJobName = HSTR("JobBase")) noexcept;

    /*
        因为DoJob是纯虚函数, 所以虚构函数是虚析构函数.
    */
    virtual ~ JobBase() noexcept;

public:
    /*
        任务的具体工作函数.
        继承类需要实现这个函数.
    */
    virtual void DoJob() = 0;


public:
    HCSTRR GetJobName() const noexcept { return m_strName; }


private:
    // job name.
    HSTR m_strName;

};


class ExitThreadJob : public JobBase {
public:
    ExitThreadJob(HCSTRR strJobName = HSTR("ExitThreadJob")) noexcept;

    void DoJob() override;

};

}

#endif //__H_UTILS_JOBBASE_H__