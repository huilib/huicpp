/*
 * Created: Joseph Hui Sat 24 Dec 2022
 * Description: 
 *      Wrapper single module object.
*/


#ifndef __H_HUICPP_SINGLE_MODULE_H__
#define __H_HUICPP_SINGLE_MODULE_H__


#include "baseinside/modulebase.h"
#include "hservlet.h"


namespace HUICPP {


class HSingleModuleBase : public BASEINSIDE::ModuleBase {
public:
    using module_base = BASEINSIDE::ModuleBase;

public:
    HSingleModuleBase(HCSTRR strName = HSTR("SingleModule")) noexcept;

    virtual ~ HSingleModuleBase() noexcept;

public:
    void EnableLog() noexcept { m_bEnableLog = true; }

    HRET InitModule(HCSTRR strJson) override;    

protected:
    virtual HRET moduleInit(HCSTRR strJson) = 0;

    void setAsInited() noexcept { m_bInited = true; }

private:
    HRET init_log();

private:
    bool m_bEnableLog;
    bool m_bInited;
};


}


#endif //__H_HUICPP_SINGLE_MODULE_H__

