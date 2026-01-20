

#include "hlog.h"
#include "htime.h"
#include "hfilename.h"
#include <cstdarg>
#include <iostream>

#include "process/pid.h"


namespace HUICPP {

typedef HSTRING_BUF log_buffer_t;
static constexpr const HN log_buffer_length = HSTRING_LEN;


HLog::HLog() noexcept :
	m_plog(nullptr) {

}


HLog::~HLog() {

	Uninit();

}


HRET HLog::Init(HCSTRR strFilename, HCSTRR strRolloverType) {

	HFileName log_fn(strFilename);
	HDirection log_dic = log_fn.GetParentDic();

	if (not log_dic.Exists()) {
		HNOTOK_RETURN(log_dic.CreateDic());
	}

	detail::log::LogfileRollbackStrategy::ROLLOVER_VALUE rt_value = 
		detail::log::LogfileRollbackStrategy::GetRolloverTypeName(strRolloverType);
	m_plog = new detail::log::LogFile(strFilename, rt_value);
	CHECK_NEWPOINT(m_plog);
	HNOTOK_MSG_RETURN(m_plog->Init(), "init log failed");

	HRETURN_OK;

}


void HLog::Uninit() {

	HDELP(m_plog);

}


HHOT_FUNCTION void HLog::WriteLog(LOG_VALUE lt, HCSTRR fn, HN ll, HCSTRR strlog) {

	detail::log::LogMessage message(strlog, fn, ll, lt);
	if (HLIKELY(m_plog != nullptr)) { 
		m_plog->Write(message); 
	} else {
		/*
			Date: July 08 2025. Joseph
			Description: 没有配置日志时, 打印输出到标准输出上.
		*/
		std::cout << message.GetMessage() << std::endl;
	}

}


static HLog g_log;
static HLog* gp_log = nullptr;


HRET InitLog(HCSTRR strFilename, HCSTRR strRolloverType) {

	IF_NOTOK(g_log.Init(strFilename, strRolloverType)) {
		printf("init log failed\n");
		exit(EXIT_FAILURE);
	}

	gp_log = &g_log;

	LOG_NORMAL("init log success with logfile[%s]", strFilename.c_str());
	
	HRETURN_OK;
}


void writelog(LOG_VALUE lt, HCSZ ff, HN ll, HCSZ fmt, ...) {

	log_buffer_t log_buf = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, log_buffer_length, fmt, ap);
	va_end(ap);

	g_log.WriteLog(lt, ff, ll, log_buf);

}


namespace inline_huicpp_logns {


LogString::LogString(HCSZ szFile, HN iLine, LOG_VALUE lt) 
	: m_logstr(), 
	m_strFile(szFile), m_iLine(iLine),
	m_lt(lt) {
}


LogString::~ LogString() {
	flush();
}


void LogString::flush () {
	g_log.WriteLog(m_lt, m_strFile, m_iLine, m_logstr.str());
}


}

} // namespace HUIBASE

