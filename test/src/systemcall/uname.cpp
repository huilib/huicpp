

#include "uname.h"

#include <sys/utsname.h>
#include <sstream>


namespace HUICPP {

HUname::HUname()
    : base_class(),
    m_strSystemName(), m_strNodeName(),
    m_strRelease(), m_strVersion(), m_strMachine() {

    struct utsname buf;
    ZERO_MEM(&buf, sizeof(struct utsname));

    SetSystemCallReturn(::uname(&buf));

    m_strSystemName.assign(buf.sysname);

    m_strNodeName.assign(buf.nodename);

    m_strRelease.assign(buf.release);

    m_strVersion.assign(buf.version);

    m_strMachine.assign(buf.machine);

}


HUname::~ HUname() {

}


HSTR HUname::ToString() const {

    std::stringstream ss;

    ss << "{\"sysname\":\"" << m_strSystemName << "\",\"nodename\":\"" << m_strNodeName << "\",\"release\":\""
        << m_strRelease << "\",\"version\":\"" << m_strVersion << "\", \"machine\":\"" << m_strMachine << "\"}";

    return ss.str();

}



}