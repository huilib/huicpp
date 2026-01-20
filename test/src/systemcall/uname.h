

#ifndef __H_SYSTEMCALL_UNAME_H__
#define __H_SYSTEMCALL_UNAME_H__

#include "systemcallbase.h"

namespace HUICPP {

class HUname final : public SystemCallBase  {
public:
    using base_class = SystemCallBase;

public:
    HUname();

    ~ HUname();

public:
    HCSTRR GetSystemName() const noexcept { return m_strSystemName;  }

    HCSTRR GetNodeName() const noexcept { return m_strNodeName;  }

    HCSTRR GetRelease() const noexcept { return m_strRelease;  }

    HCSTRR GetVersion() const noexcept { return m_strVersion;  }

    HCSTRR GetMachine() const noexcept { return m_strMachine;  }

    HSTR ToString() const override;

private:
    HSTR m_strSystemName;
    HSTR m_strNodeName;
    HSTR m_strRelease;
    HSTR m_strVersion;
    HSTR m_strMachine;
};

}


#endif // __H_SYSTEMCALL_UNAME_H__

