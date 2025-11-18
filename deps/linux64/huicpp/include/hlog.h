/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp log.
*/

#ifndef __H_HUICPP_LOG_H__
#define __H_HUICPP_LOG_H__

#include "huicpp.h"
#include "detail/log/logfile_detail.h"
#include <sstream>

namespace HUICPP {

using LOG_TYPE = HUICPP::detail::log::LogPriority;
using LOG_VALUE = HUICPP::detail::log::LogPriority::PriorityValue;	

class HLog {
public:
	HLog() noexcept;

	virtual ~HLog();

public:
	HRET Init(HCSTRR strFilename, HCSTRR strRolloverType = HSTR("DAY"));

	void Uninit();

	HHOT_FUNCTION void WriteLog(LOG_VALUE lt, HCSTRR fn, HN ll, HCSTRR strlog);

private:
	detail::log::LogFile* m_plog;
};


HRET InitLog(HCSTRR strFilename, HCSTRR strRolloverType = HSTR("DAY"));

void writelog(LOG_VALUE lt, HCSZ ff, HN ll, HCSZ fmt, ...);

#define LOG_NORMAL(fmt, ...) writelog(LOG_TYPE::INFO, __FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define LOG_WARNING(fmt, ...) writelog(LOG_TYPE::WARNING, __FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define LOG_ERROR(fmt, ...) writelog(LOG_TYPE::ERROR, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

#define SLOG_NORMAL(fmt, ...) writelog(LOG_TYPE::INFO, __FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define SLOG_WARNING(fmt, ...) writelog(LOG_TYPE::WARNING, __FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define SLOG_ERROR(fmt, ...) writelog(LOG_TYPE::ERROR, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

#define FUN_BEGIN LOG_NORMAL("[%s]: begin...", __FUNCTION__)
#define FUN_END LOG_NORMAL("[%s]: end...", __FUNCTION__)

namespace inline_huicpp_logns {

class LogString {
public:
	LogString(HCSZ szFile, HN iLine, LOG_VALUE lt = LOG_TYPE::INFO);

	~ LogString();

	LogString(const LogString&) = delete;

	LogString& operator= (const LogString&) = delete;

public:
	std::ostream & Stream() { return m_logstr; }

private:
	void flush ();	

private:
	std::stringstream m_logstr;
	HSTR m_strFile;
	HN m_iLine;
	LOG_VALUE m_lt;
};

}

#define COMPACT_HUICPP_LOG_INFO inline_huicpp_logns::LogString(__FILE__, __LINE__, LOG_TYPE::INFO)
#define COMPACT_HUICPP_LOG_WARN inline_huicpp_logns::LogString(__FILE__, __LINE__, LOG_TYPE::WARNING)
#define COMPACT_HUICPP_LOG_ERROR inline_huicpp_logns::LogString(__FILE__, __LINE__, LOG_TYPE::ERROR)

#define HLOG(log_type) COMPACT_HUICPP_LOG_ ## log_type.Stream()

} // namespace HUICPP

#endif //__H_HUICPP_LOG_H__
