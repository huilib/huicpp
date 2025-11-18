/*
 * Created Author: Joseph Hui
 * Created Date: Mon 6 Jun 2022
 * Description:
 *      Wrapper servlet.
*/

#ifndef __H_HUICPP_SERVLET_H__
#define __H_HUICPP_SERVLET_H__

#include "happ.h"
#include <atomic>

namespace HUICPP {

class HServletBase {

protected:
    explicit HServletBase(HCSTRR strName = HSTR("HServletBase"), HApp* app = nullptr) noexcept;

    HServletBase(const HServletBase&) = delete;

    HServletBase(HServletBase&&) = delete;

    virtual ~HServletBase() noexcept ;

public:
    virtual HRET InitServlet();

    virtual HRET ServletRun () = 0;
    
protected:
    // only invoke in or inherit this class.
    void setasInited() noexcept { m_bInited = true; }
    void setUninited() noexcept { m_bInited = false; }

public:    
    bool IsInited() const noexcept { return m_bInited; }
    HApp* GetApp() noexcept { return m_app; }
    const HApp* GetApp() const noexcept { return m_app; } 
    
private:
    bool m_bInited;
    HApp* m_app;
    HSTR m_strName;
};



}

#endif //__H_HUICPP_SERVLET_H__

