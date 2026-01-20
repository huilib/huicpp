

#include "hservlet.h"
#include <hlog.h>

namespace HUICPP {

HServletBase::HServletBase(HCSTRR strName, HApp* app) noexcept
    : m_bInited(false), m_app(app), m_strName(strName) {  }


HServletBase::~HServletBase() noexcept { }


HRET HServletBase::InitServlet () {

    HASSERT_RETURN(not IsInited(), ERR_STATUS);

    setasInited();

    HRETURN_OK;
}

}

