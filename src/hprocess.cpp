
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

#include "hstr.h"

#include "hprocess.h"
#include "hstr.h"
#include "hlog.h"


namespace HUICPP {

bool HProcess::ProcessExists (const pid_type& pid) {
	HSTR str = HStr::Format("/proc/%d", pid);
	struct stat st;
	ZERO_MEM(&st, sizeof(struct stat));
	return stat(str.c_str(), &st) == 0;
}


void HProcess::KillProcessAnyway (const pid_type& pid) {
	HIGNORE_RETURN(kill_process(pid, SIGKILL));
}


HRET HProcess::KillProcess (const pid_type& pid) {
	return kill_process(pid, SIGTERM);
}


HRET HProcess::GetProcName(HSTRR strName) {
    HCSZ csz = "/proc/self/exe";
	HNAME_BUF buf = {0};
	HASSERT_RETURN(readlink(csz, buf, HNAME_LEN) >= 0, SYS_FAILED);
	strName.assign(buf);
    HRETURN_OK;
}


void HProcess::chlsig_handler(int) {
	pid_t pid = 0;
	int stat = 0;
	while ((pid =  waitpid(-1, &stat, WNOHANG)) > 0) {
		SLOG_WARNING("son process-id[%d] exit", pid);
	}
}


HRET HProcess::kill_process(const pid_type& pid, SYS_T signo) {
	auto cb = kill(pid, signo);
	HASSERT_RETURN(cb == 0, SYS_FAILED);		
	HRETURN_OK;
}


}










