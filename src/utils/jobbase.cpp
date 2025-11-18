

#include "jobbase.h"

#include <pthread.h>

namespace HUICPP {

JobBase::JobBase(HCSTRR strJobName) noexcept
    : m_strName(strJobName) {

}


JobBase::~JobBase() noexcept {


}


ExitThreadJob::ExitThreadJob(HCSTRR strJobName) noexcept
    : JobBase(strJobName) { 

}


void ExitThreadJob::DoJob() {

    pthread_exit(nullptr);

}


}

