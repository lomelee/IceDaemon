////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 服务启动入口
/// @par 相关文件
/// DeamonService.h
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         IceDaemonMain.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DaemonService.h"
#include "DaemonServiceDef.h"


using namespace ICEDAEMON;

#ifndef _DEBUG
void main(int argc, char* argv[])
{
	/// 设置当前运行目录
	e_SetCurDirectory();
	// 设置读取的配置文件
	CRWIniFile rwIniFile;
	rwIniFile.e_InitRWFile(CONFIG_FILE_NAME);
	// 字符串服务名称
	string strServiceName = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_NAME_KEY);
	// 字符串服务名称描述
	string strServiceDis = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_DISPLAY);
	char szServiceName[128];
	memset(szServiceName, 0x00, sizeof(szServiceName));
	strncpy(szServiceName, strServiceName.c_str(), sizeof(szServiceName) - 1);

	// 注册服务
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		// 服务配置
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)CDaemonService::e_ServiceMain },
		// 结束信息
		{ NULL, NULL }
	};
	
	CDaemonService CThisService;
	CThisService.e_IniConfig(strServiceName.c_str(), strServiceDis.c_str(), &rwIniFile);
	if(false == CThisService.e_ParseStandardArgs(argc, argv))
	{
		/// 启动服务的控制分派机线程
		if(FALSE == StartServiceCtrlDispatcher(DispatchTable))
		{
			CDaemonService::e_LogEvent(EVENTLOG_ERROR_TYPE, "StartServiceCtrlDispatcher Fails...");
		}
	}
}

#else

int main(int argc, char* argv[])
{
	/// 设置当前运行目录
	e_SetCurDirectory();

	// 设置读取的配置文件
	CRWIniFile rwIniFile;
	rwIniFile.e_InitRWFile(CONFIG_FILE_NAME);
	// 字符串服务名称
	string strServiceName = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_NAME_KEY);
	// 字符串服务名称
	string strServiceDis = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_DISPLAY);

	/// 定义服务调用对象
	CProcessMonitor processMonitor;
	// 获取执行文件配置信息
	string strExecName = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_NAME);
	string strExecPath = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_PATH);
	string strExecArgv = rwIniFile.e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_ARGV);
	/// 初始化启动服务
	if(true == processMonitor.e_StartMonitor(strExecName.c_str(), strExecPath.c_str(), strExecArgv.c_str()))
	{
		char szchar;
		memset(&szchar, 0x00, sizeof(szchar));
		while(szchar = getchar())
		{
			if('0' == szchar)
			{
				// 停止进程
				processMonitor.e_StopProcessRun();
				break;
			}
		}
	}
	return 0;
}
#endif
