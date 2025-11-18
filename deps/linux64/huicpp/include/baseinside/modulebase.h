/*
 * Created: Joseph Hui Sat 24 Dec 2022
 * Description: 
 *      Wrapper module object.
*/


#ifndef __H_HUICPP_BASEINSIDE_MODULEBASE_H__
#define __H_HUICPP_BASEINSIDE_MODULEBASE_H__

#include "../huicpp.h"
#include "../utils/json_utils.h"

namespace HUICPP {


namespace BASEINSIDE {


class ModuleBase {
public:
    ModuleBase(HCSTRR strModuleName = "ModuleBase") noexcept;

    virtual ~ ModuleBase() noexcept;

public:
    virtual HRET InitModule(HCSTRR strJson) = 0;

    virtual void ExitModule () = 0;

public:
    HCSTRR GetModuleName() const noexcept { return m_strModuleName; }

private:
    HCSTR m_strModuleName;
};


}


}


#endif //__H_HUICPP_BASEINSIDE_MODULEBASE_H__
