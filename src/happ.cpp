
#include "happ.h"
#include "hlog.h"
#include <getopt.h>
#include "hstr.h"
#include "hfilestr.h"
#include "hfilename.h"
#include "systemcall/uname.h"

#include <signal.h>
#include <unistd.h>


namespace HUICPP {

// 主要用于信号注册函数.
static HApp* gapp = nullptr;

// HAPP 需要处理的命令行参数
static HCSZ s_cmdopts = "c:d:h";
static const struct  option s_opts[] = {
    { "config", 1, nullptr, 'c' },
    { "deamon", 1, nullptr, 'd' },
    { "help", 1, nullptr, 'h' },
    { nullptr, 0, nullptr, 0 }
};


HApp::HApp() noexcept
    : m_bDaemon(true), m_bInited(false), m_bRunning(true),
        m_conf(), m_app_lock(nullptr) {
    
    initApp();    

}


HApp::HApp(HN argc, HCSZ argv[])
    : m_bDaemon(true), m_bInited(false), m_bRunning(true),
        m_conf(), m_app_lock(nullptr) { 
     
    anaOpt(argc, argv);

}


HApp::~HApp () {

}


HRET HApp::Init(){

	setup_signal();

	gapp = this;

    HNOTOK_MSG_RETURN(setup_app_dir(), "app setup direction failed");

    HNOTOK_MSG_RETURN(setup_config(), "app setup configuration failed");
	
	HNOTOK_MSG_RETURN(setup_log(), "app setup log failed");

    SLOG_NORMAL("home-dir[%s] conf[%s] log[%s]", 
        m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_HOMEPATH).c_str(),
    	m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_CONF).c_str(), 
        m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_LOG).c_str());

    HNOTOK_MSG_RETURN(setup_daemon(), "app setup as daemon failed");

    HNOTOK_MSG_RETURN(setup_pidfile(), "setup pid file failed");
    
	// process lock
    HNOTOK_MSG_RETURN(setup_applock(), "app lock failed");	

    HRETURN_OK;
}


bool HApp::Run() {

    while(HLIKELY(IsRunning())) {

        SLOG_NORMAL("HApp run... Please override this function.");
        sleep(1);

    }

    return true;

}


void HApp::Handle_sig(int sig) {

    SLOG_ERROR("===========================signal [%d]======================", sig);

}


HCSTRR HApp::GetArgvByIndex(HN index) const {
    HSTR strKey = HStr::Format("%s_%d", AppDefaultKeyValues::CONF_KEY_ARGS, index);
    return m_conf.GetValue(strKey);
}


HN HApp::GetArgvCount() const {
    HCSTRR strCount = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_ARGS_COUNT);
    return HStr(strCount).ToN();
}


void HApp::initApp () {

    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_CONF, AppDefaultKeyValues::DEF_CONF_VAL_CONF);
    
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_EXE_PATH, HDirection().SetAsCurrent().GetName());

}


void HApp::anaOpt(int argc, HCSZ argv[]){

    // program filepath.
    HSTR strProgram(*argv);

    // setup EXE name to config.
    HFileName fnProgram(strProgram);
    HSTR strProgramName = fnProgram.FileName();
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_EXE_NAME, strProgramName);

    HSTR strProgramDir;
    HDirection pro_dic(strProgram);
    if (not pro_dic.IsAbsoluteDic()) {
        HDirection cur_dic;
        cur_dic.SetAsCurrent();
        strProgramDir = cur_dic.GetName();
        strProgram = cur_dic.GetDicFile(strProgramName).GetName();
    } else {
        strProgramDir = pro_dic.GetParent().GetName();
    }

    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_EXE_FILEPATH, strProgram);
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_EXE_PATH, strProgramDir);

    int cb = 0;
    while ((cb = getopt_long(argc, const_cast<char**>(argv), 
        s_cmdopts, s_opts, nullptr)) != -1 ){
        switch (cb) {
            case 'c':
                m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_CONF, optarg);
                break;
            case 'd':
                m_bDaemon = true;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
                break;        
            default:                    
                break;
        }
    }

    for (HN i = 0; i < argc; ++i) {
        HCSZ szarg(argv[i]);
        HSTR arg_key = HStr::Format("%s_%d", AppDefaultKeyValues::CONF_KEY_ARGS, i);
        m_conf.SetValue(arg_key, szarg);
    }
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_ARGS_COUNT, HStr::FromN(argc));
}


void HApp::help() const noexcept {

    printf("Huicpp Frameword, System Information: %s\n", HUname().ToString().c_str());
    printf("Please override this function to print help message\n");

}


void HApp::setup_signal() const {

    signal(SIGTERM, stop_handle);   // KILL PID (NOT KILL -9 PID)
    signal(SIGINT, stop_handle);    // CTRL+C
    signal(SIGSEGV, bug_handle);

    signal(SIGPIPE, SIG_IGN);       // network broken. 
                                    // So that clients on the same host that are killed don't also kill us.
    
    // signal(SIGPIPE, nullptr);

    signal(SIGILL, stop_handle);
    signal(SIGFPE, stop_handle);
    signal(SIGABRT, stop_handle);

}


HRET HApp::setup_app_dir() {

    HASSERT_RETURN(m_conf.Exists(AppDefaultKeyValues::CONF_KEY_EXE_PATH), SRC_ERR);

    HCSTRR strExePath = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_EXE_PATH);
    HDirection dir(strExePath);

    HSTR home_dir = dir.GetParent().GetName();
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_HOMEPATH, home_dir);

    // setup data dir to config.
    HDirection data_dir(home_dir);
    data_dir.Append("data");
    HSTR strDataDir = data_dir.GetName();
    if (not data_dir.Exists()) {
        IF_NOTOK(data_dir.CreateDic()) {
            printf("create data dir FAILED\n");
            exit(EXIT_FAILURE);
        }
    }
    m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_DATA_PATH, AppDefaultKeyValues::DEF_CONF_VAL_DATA_PATH);

    HRETURN_OK;
}


HRET HApp::setup_config() {

    // if there is not a config key in the config, 
    // we setup the 'conf/conf' to it.
    if (not m_conf.Exists(AppDefaultKeyValues::CONF_KEY_CONF)) {
        HDirection home_dir(m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_HOMEPATH));
        HDirection conf_dir = home_dir.Append("conf/conf");
        m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_CONF, conf_dir.GetName());
    }

    // load config file.
    IF_NOTOK(m_conf.LoadConfig(m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_CONF))) {
        printf("Load Config File Fialed.\n");
    }

    HRETURN_OK;
}


HRET HApp::setup_log () {

    if (not m_conf.Exists(AppDefaultKeyValues::CONF_KEY_LOG)) {
        HRETURN_OK;
    }

    /*
        get log file name.
    */
    HSTR strLogFileName;

    HSTR strLogType = m_conf.GetValue("CK_logtype", "DAY");

	if (m_conf.Exists(AppDefaultKeyValues::CONF_KEY_LOG)) {
        // if log path is in config, we take from it.
		strLogFileName = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_LOG);	    
	} else {
        // make log path.
        HDirection home_dir(m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_HOMEPATH));
        HDirection log_dir = home_dir.Append("log");

        // make log file name.
        HSTR strExeName = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_EXE_NAME);
        strExeName = HFileName(strExeName).FileNameOnly();
        if (strExeName.empty()) {
            // setup default log name: huicpp_pid.
            strExeName = HStr::Format("huicpp_%d", HTO_INT(getpid()));
        }
        HIGNORE_RETURN(HStr::Lower(strExeName));
        HFileName fnLog = log_dir.GetDicFile(strExeName);
		m_conf.SetValue(AppDefaultKeyValues::CONF_KEY_LOG, fnLog.GetName());
	}

	if(strLogFileName.empty()){      
	    strLogFileName = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_LOG);
	}
    
    return InitLog(strLogFileName, strLogType);

}


HRET HApp::setup_daemon() {

    HStr dea_str;
	if (m_conf.Exists(AppDefaultKeyValues::CONF_KEY_DAEMON)) {
		dea_str = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_DAEMON);
        dea_str.Trim();
	    SLOG_NORMAL("whether daemon instance [%s]", dea_str.c_str());
	    if (dea_str == "1") {
	        SLOG_NORMAL("set as daemon");
	        setas_daemon_app();
	    }
	}

    HRETURN_OK;
}


HRET HApp::setup_pidfile() {

    HCSTRR strExeName = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_EXE_NAME, "jspp");
    HSTR strPidFileName = HStr::Format("../%s/%s.pid", AppDefaultKeyValues::DEF_CONF_VAL_DATA_PATH, strExeName.c_str());
    HSTR strPid = HStr::Format("%d", getpid());
    HFileStr fs;
    HNOTOK_MSG_RETURN(fs.OutOpen(strPidFileName), "create pid file failed");
    HNOTOK_MSG_RETURN(fs.WriteFromString(strPid), "write pid to pidfile FAILED");

    HRETURN_OK;
    
}


HRET HApp::setup_applock() {

    HStr applock_str;
	if (m_conf.Exists(AppDefaultKeyValues::CONF_KEY_APPLOCK)) {
		applock_str = m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_APPLOCK, "1");
        applock_str.Trim();
	    SLOG_NORMAL("whether lock instance [%s]", applock_str.c_str());
	    if (applock_str == "1") {
	        lock_app();
	    }
	}

    HRETURN_OK;
}


void HApp::setas_daemon_app(){
        
    if (fork() > 0) {	
        exit(0);    
    }    

    setsid();   

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN); 

    if (fork() > 0) {
        exit(0);
    }
    
    umask(0);
    LOG_NORMAL("daemon process no:[%d]", getpid());

}


void HApp::lock_app () {

    HSTR strLockFileName = HSTR(".") +  HFileName(m_conf.GetValue(AppDefaultKeyValues::CONF_KEY_EXE_NAME)).FileNameOnly();
    
    m_app_lock = std::make_shared<HAppLock>(strLockFileName);
    CHECK_NEWPOINT(m_app_lock);

    HNOTOK_MSG_THROW(m_app_lock->LockApp(), "application lock failed. Multiple application instance have run");
    
}


void HApp::stop_handle(int sig) {

    SLOG_WARNING("stop main process with sig[%d]", sig);
	if (gapp) {        
		gapp->Stop();
	}

}


void HApp::bug_handle (int sig) {

    if(gapp) {
        gapp->Handle_sig(sig);
        
        //gapp->Stop();
        // we don't stop app. casuse app is a main daemon app.
        // we just exit process.
        LOG_NORMAL("app exit with code[%d]", sig);
        exit(sig);
    }

}


}

