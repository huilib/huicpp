/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp application.
*/


#ifndef __H_HUICPP_APPLICATION_H__
#define __H_HUICPP_APPLICATION_H__

#include "huicpp.h"
#include "hnormalconf.h"
#include "happlock.h"

#include <atomic>
#include <memory>

namespace HUICPP {


class HApp{
public:
	HApp() noexcept;

	HApp(HN argc, HCSZ argv[] );

	virtual ~ HApp();

public:
	virtual HRET Init();

	virtual bool Run();			
	
	virtual void Stop () { m_bRunning.store(false, std::memory_order::memory_order_release); }

	bool IsRunning () const { return m_bRunning.load(std::memory_order::memory_order_acquire); }

	void Handle_sig (int sig);

public:
	HCSTRR GetArgvByIndex(HN index) const;

	HN GetArgvCount() const;

protected:
	// prepare initialize with default params.
	void initApp ();

	/*
		分析命令行参数, 并设置到m_conf上;
		virtual 可以被继承类重写;
	*/
	virtual void anaOpt(int argc, HCSZ argv[]);

	virtual void help() const noexcept;
	
	virtual void setup_signal() const;

	/*
		setup application running directory.
		In this method, it maybe create a data directory.
	*/
	HRET setup_app_dir();

	/*
		Load config file which indicated in config matched with 'CONF_KEY_CONF'
	*/
	virtual HRET setup_config();

	HRET setup_log () ;

	HRET setup_daemon();

	HRET setup_pidfile();

	HRET setup_applock();
	

protected:
	virtual void setas_daemon_app();

	virtual void lock_app ();

public:
	static void stop_handle(int sig);
	static void bug_handle(int sig);

public:
	// config api.
	HCSTRR GetConfValue (HCSTRR key) const { return m_conf.GetValue(key); }
	HCSTRR GetConfValue (HCSTRR key, HCSTRR defVal) const noexcept 
	{ return m_conf.GetValue(key, defVal); }
	HN GetConfIntValue (HCSTRR key) const 
	{ return m_conf.GetIntValue(key); }
	HN GetConfIntValue (HCSTRR key, HN def) const noexcept 
	{ return m_conf.GetIntValue(key, def); }
	bool ConfExists(HCSTRR strKey) const { return m_conf.Exists(strKey); }

	const HNormalConfig& GetGlobalConfig() const noexcept { return m_conf; }

public:
	struct AppDefaultKeyValues {
		// follows is default config keys and values.

		// keys:
		// the default key for config filename.
		static constexpr HCSZ CONF_KEY_CONF = "CK_conf";

		// the default config key for exe file path.
		static constexpr HCSZ CONF_KEY_EXE_FILEPATH = "CK_exe_filepath";

		// execute file name.
		static constexpr HCSZ CONF_KEY_EXE_NAME = "CK_exe_name";

		// log file name.
		static constexpr HCSZ CONF_KEY_LOG = "CK_log";

		// the default config key for exe path.
		static constexpr HCSZ CONF_KEY_EXE_PATH = "CK_exe_path";
		
		// the main directory of this application.
		static constexpr HCSZ CONF_KEY_HOMEPATH = "CK_homepath";
		static constexpr HCSZ CONF_KEY_DAEMON = "CK_isdaemon";
		static constexpr HCSZ CONF_KEY_APPLOCK = "CK_applock";
		static constexpr HCSZ CONF_KEY_LOGTYPE = "CK_logtype";
		
		static constexpr HCSZ CONF_KEY_ARGS = "CK_argv";
		static constexpr HCSZ CONF_KEY_ARGS_COUNT = "CK_argv_count";	
		static constexpr HCSZ CONF_KEY_DATA_PATH = "CK_data_path";

		// values:
		// the default config file filepath.
		static constexpr HCSZ DEF_CONF_VAL_CONF = "conf/conf";		
		
		static constexpr HCSZ DEF_CONF_VAL_DATA_PATH = "data";
	};	
	
private:
	bool m_bDaemon;												// whether run as daemon
	bool m_bInited;												// whether HApp has already initialized.

	// HApp running status.
	std::atomic<bool> m_bRunning;	
	HNormalConfig m_conf;										// application config
	std::shared_ptr<HAppLock> m_app_lock;						// application lock

};

}

#endif //__H_HUICPP_APPLICATION_H__

