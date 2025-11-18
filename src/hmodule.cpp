

#include "hmodule.h"

#include <hlog.h>
#include <hfilename.h>

namespace HUICPP {


HSingleModuleBase::HSingleModuleBase(HCSTRR strName) noexcept
    : module_base (strName), m_bEnableLog(false), m_bInited(false) {

}


HSingleModuleBase::~ HSingleModuleBase() noexcept {

}



HRET HSingleModuleBase::InitModule(HCSTRR strJson) {

    if (not m_bInited and m_bEnableLog) {
        HNOTOK_RETURN(init_log());
    }
    
    RET_T ret = moduleInit(strJson);

    setAsInited();

    return ret;

}


HRET HSingleModuleBase::init_log() {

    HSTR strLogType("10MIN");

    HDirection log_dir;
    log_dir.SetAsCurrent();
    HFileName log_fn = log_dir.GetDicFile(GetModuleName());
    
    return InitLog(log_fn.GetName(), strLogType);

}


}


