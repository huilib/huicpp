

#include "hexception.h"
#include "hstr.h"
#include "adaptation/ada_error.h"

namespace HUICPP {

	HException::HException(HN err, HCSZ szFile, HN iLineNo) noexcept
		: m_str(), m_err(err), 
		m_strFile(szFile), m_iLineno(iLineNo) {

		initmsg();
		
	}

	HException::HException(HCSTRR strErrMsg, 
		HN err, HCSZ szFile, HN iLineNo) noexcept
		: m_str(strErrMsg), m_err(err), 
		m_strFile(szFile), m_iLineno(iLineNo) {
		
		initmsg();

	}


	HException::~HException() noexcept {
	}


	HSTR HException::GetSysError(HN errnum) {

		HWORD_BUF buf = {0};
		HIGNORE_RETURN(adapt::hstrerror(errnum, buf, HWORD_LEN));
		return HSTR(buf);

	}


	void HException::initmsg() {

		m_str.clear();

		m_str.append("huicpp exception msg:[");
		m_str.append(hbase_err(m_err));
		m_str.append("] ");

		m_str.append("System error msg:[");
		m_str.append(GetSysError());
		m_str.append("] ");

		appendLocationInfo();

	}


	void HException::appendLocationInfo() {

		m_str.append(HStr::Format("[%s:%d]", m_strFile.c_str(), m_iLineno));

	}

}
