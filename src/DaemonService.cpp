////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 服务模块头文件
/// @par 相关文件
/// CDaemonService.h
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         CDaemonService.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DaemonService.h"
#include "DaemonServiceDef.h"

namespace ICEDAEMON
{
	CDaemonService* pThisService = NULL;
	CDaemonService::CDaemonService()
	{
		pThisService = this;
		m_hSvcStopEvent = NULL;
		m_pRWIniFile = NULL;
		m_strServiceName = "";
	}

	CDaemonService::~CDaemonService()
	{
		m_pRWIniFile = NULL;
		m_strServiceName = "";
	}

	void WINAPI CDaemonService::e_ServiceMain(DWORD dwArgc, LPSTR* lpszArgv)
	{
		/// 验证数据合法性
		if(NULL == pThisService)
		{
			CDaemonService::e_LogEvent(EVENTLOG_ERROR_TYPE, "e_ServiceMain pThisService Is Null");
			return;
		}

		/// 为服务注册通知函数
		pThisService->m_SvcStatusHandle = ::RegisterServiceCtrlHandler(pThisService->m_strServiceName.c_str(), e_SvcCtrlHandler);
		if(!pThisService->m_SvcStatusHandle)
		{
			CDaemonService::e_LogEvent(EVENTLOG_ERROR_TYPE, "RegisterServiceCtrlHandler");
			return;
		}

		/// 只有一个单独的服务
		pThisService->m_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		pThisService->m_SvcStatus.dwServiceSpecificExitCode = 0;
		/// 上报初始化状态到服务管理器
		pThisService->e_ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

		/// 创建服务停止事件
		pThisService->m_hSvcStopEvent = CreateEvent(
			NULL,    // default security attributes
			TRUE,    // manual reset event
			FALSE,   // not signaled
			NULL);   // no name

		/// 事件创建失败返回，并通知 Windows 停止服务
		if(NULL == pThisService->m_hSvcStopEvent)
		{
			pThisService->e_ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
			return;
		}
		/// 运行服务
		pThisService->e_ServerProgram(dwArgc, lpszArgv);
		/// 上报服务停止状态
		pThisService->e_ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
	}

	/// 服务主程序
	bool CDaemonService::e_ServerProgram(DWORD dwArgc, LPSTR* lpszArgv)
	{
		/// 定义服务调用对象
		CProcessMonitor processMonitor;
		// 获取执行文件配置信息
		string strExecName = m_pRWIniFile->e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_NAME);
		string strExecPath = m_pRWIniFile->e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_PATH);
		string strExecArgv = m_pRWIniFile->e_ReadConfigFromFile(SERVICE_MOUDLE_KEY, SERVICE_EXEC_ARGV);
		/// 初始化启动服务
		if(true == processMonitor.e_StartMonitor(strExecName.c_str(), strExecPath.c_str(), strExecArgv.c_str()))
		{
			/// 启动服务失败
			e_ServiceRun();
			/// 服务停止时, 停止需要守护的进程
			processMonitor.e_StopProcessRun();
		}
		else
		{
			/// 启动服务失败
			CDaemonService::e_LogEvent(EVENTLOG_ERROR_TYPE, "e_ServerProgram pThisService Start Fails.");
		}		
		return true;
	}

	void CDaemonService::e_ServiceRun()
	{
		while(1)
		{
			/// 上报服务运行状态
			e_ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
			/// 验证服务
			::WaitForSingleObject(m_hSvcStopEvent, INFINITE);
			return;
		}
	}

	void CDaemonService::e_ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
	{
		static DWORD dwCheckPoint = 1;
		/// 赋值服务状态信息
		m_SvcStatus.dwCurrentState = dwCurrentState;
		m_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
		m_SvcStatus.dwWaitHint = dwWaitHint;
		/// 赋值加载状态
		if(SERVICE_START_PENDING == dwCurrentState)
		{
			m_SvcStatus.dwControlsAccepted = 0;
		}
		else
		{
			m_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		}

		/// 设置上报次数
		if((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
		{
			m_SvcStatus.dwCheckPoint = 0;
		}
		else
		{
			m_SvcStatus.dwCheckPoint = dwCheckPoint++;
		}

		/// 上报服务状态信息
		::SetServiceStatus(m_SvcStatusHandle, &m_SvcStatus);
	}

	void WINAPI CDaemonService::e_SvcCtrlHandler(DWORD dwCtrl)
	{
		/// 服务管理信号
		switch(dwCtrl)
		{
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			{
				/// 打印信息
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "Rev Stop OR Shutdown Message.");
				/// 上报服务停止中
				pThisService->e_ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
				// 打印事件
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "%s Service Stoping1 CtrlEvenType = %ld", pThisService->m_strServiceName.c_str(), dwCtrl);
				/// 设置停止服务信号量
				::SetEvent(pThisService->m_hSvcStopEvent);
				return;
			}
		case SERVICE_CONTROL_PRESHUTDOWN:
		case SERVICE_CONTROL_PAUSE:
			{
				/// 打印信息
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "Rev PAUSE Or PRESHUTDOWN Message.");
				/// 上报服务停止中
				pThisService->e_ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
				// 打印事件
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "%s Service Stoping2 CtrlEvenType = %ld", pThisService->m_strServiceName.c_str(), dwCtrl);
				/// 设置停止服务信号量
				::SetEvent(pThisService->m_hSvcStopEvent);
				return;
			}

		case SERVICE_CONTROL_CONTINUE:
			{
				/// 打印信息
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "e_SvcCtrlHandler Rev Stop Message.");
				/// 上报服务停止中
				pThisService->e_ReportSvcStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);
				// 打印事件
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "%s Service Continue3 CtrlEvenType = %ld", pThisService->m_strServiceName.c_str(), dwCtrl);
				/// 设置停止服务信号量
				// ::SetEvent(pThisService->m_hSvcStopEvent);
				return;
			}
		case SERVICE_CONTROL_INTERROGATE: 
			{
				/// 打印信息
				CDaemonService::e_LogEvent(EVENTLOG_INFORMATION_TYPE, "%s 上报当前服务状态", pThisService->m_strServiceName.c_str());
				/// 查问，需要报告当前服务状态
				pThisService->e_ReportSvcStatus(pThisService->m_SvcStatus.dwCurrentState, NO_ERROR, 0);
				break;
			}
		default:
			/// 默认也上报一下当前服务状态
			pThisService->e_ReportSvcStatus(pThisService->m_SvcStatus.dwCurrentState, NO_ERROR, 0);
			break;
		}		
	}

	void CDaemonService::e_LogEvent(WORD wType, const char* pszFormat, ...)
	{
		HANDLE hEventSource;
		LPCTSTR lpszStrings[2];
		/// 错误缓冲
		const UINT LOG_EVENT_MSG_SIZE = 1024;
		char szBuffer[LOG_EVENT_MSG_SIZE] = {0};
		/// Format处理
		va_list argptr;
		va_start(argptr, pszFormat);
		// 必须要在va_start后初始化对象
		memset(szBuffer, 0x00, LOG_EVENT_MSG_SIZE);
		vsnprintf_s(szBuffer, LOG_EVENT_MSG_SIZE - 1, pszFormat, argptr);
		va_end(argptr);

		/// 设置上报来源
		hEventSource = RegisterEventSource(NULL, pThisService->m_strServiceName.c_str());
		if(NULL != hEventSource)
		{
			lpszStrings[0] = pThisService->m_strServiceName.c_str();
			lpszStrings[1] = szBuffer;

			ReportEvent(hEventSource,// event log handle
				wType,				 // event type /// EVENTLOG_INFORMATION_TYPE | EVENTLOG_ERROR_TYPE
				0,                   // event category
				SVC_ERROR,           // event identifier Or 0
				NULL,                // no security identifier
				2,                   // size of lpszStrings array
				0,                   // no binary data
				lpszStrings,         // array of strings
				NULL);               // no binary data
			DeregisterEventSource(hEventSource);
		}
	}

	bool CDaemonService::e_IsInstalled()
	{
		bool bResult = false;
		/// 打开服务管理器,如果函数成功，返回值是一个指定的服务控制管理器数据库的句柄。如果函数失败，返回值为NULL

		SC_HANDLE hSCM = ::OpenSCManager(NULL,	// local machine(第一个参数为空表示连接本地服务管理器)
			NULL,								// ServicesActive database(指定将要打开的服务控制管理数据库的名称,如果该指针为NULL,则打开默认的SERVICES_ACTIVE_DATABASE)
			SC_MANAGER_ALL_ACCESS);				// full access(指定服务访问控制管理器的权限。)

		if(hSCM)
		{
			// Try to open the service
			SC_HANDLE hService = ::OpenService(hSCM,
				(LPCSTR)m_strServiceName.c_str(),
				SERVICE_QUERY_CONFIG);
			if(hService)
			{
				bResult = true;
				::CloseServiceHandle(hService);
			}
			::CloseServiceHandle(hSCM);
		}
		return bResult;
	}

	bool CDaemonService::e_Install()
	{
		/// 打开服务管理器
		SC_HANDLE hSCM = ::OpenSCManager(NULL,	// local machine
			NULL,								// ServicesActive database
			SC_MANAGER_ALL_ACCESS);				// full access
		if(!hSCM)
		{
			return false;
		}

		/// 获取当前路径
		TCHAR szFilePath[_MAX_PATH];
		::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

		// 创建服务
		SC_HANDLE hService = ::CreateService(hSCM,
			(LPCSTR)m_strServiceName.c_str(),
			(LPCSTR)m_strServiceName.c_str(),
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,	//SERVICE_INTERACTIVE_PROCESS与桌面交互
			SERVICE_AUTO_START,											//SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			(LPCSTR)szFilePath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		if(!hService)
		{
			::CloseServiceHandle(hSCM);
			return false;
		}

		/// 写入注册表
		char szKey[256];
		HKEY hKey = NULL;
		strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
		strcat_s(szKey, m_strServiceName.c_str());
		if (::RegCreateKey(HKEY_LOCAL_MACHINE, (LPCSTR)szKey, &hKey) != ERROR_SUCCESS) {
			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
			return false;
		}
		/// 设置键值
		::RegSetValueEx(hKey,
			"EventMessageFile",
			0,
			REG_EXPAND_SZ, 
			(const BYTE *)&szFilePath,
			strlen(szFilePath)*2 + 1);

		// Set the supported types flags.
		DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
		::RegSetValueEx(hKey,
			"TypesSupported",
			0,
			REG_DWORD,
			(CONST BYTE*)&dwData,
			sizeof(DWORD));
		::RegCloseKey(hKey);

		char szTempServiceDis[256];
		memset(szTempServiceDis, 0x00, sizeof(szTempServiceDis) - 1);
		strncpy(szTempServiceDis, m_strServiceDis.c_str(), sizeof(szTempServiceDis) - 1);
		/// 添加服务描述
		SERVICE_DESCRIPTION sdBuf;
		sdBuf.lpDescription = szTempServiceDis;
		if(FALSE == ::ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sdBuf))
		{
			e_LogEvent(EVENTLOG_ERROR_TYPE, "SERVICE_CONFIG_DESCRIPTION ERROR");
		}

		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return true;
	}

	bool CDaemonService::e_Uninstall()
	{
		/// 打开服务管理器
		SC_HANDLE hSCM = ::OpenSCManager(
			NULL,					// local machine
			NULL,					// ServicesActive database
			SC_MANAGER_ALL_ACCESS); // full access

		if(!hSCM)
		{
			return false;
		}

		bool bResult = false;
		SC_HANDLE hService = ::OpenService(hSCM,
			(LPCSTR)m_strServiceName.c_str(),
			DELETE);
		if(hService)
		{
			if(::DeleteService(hService))
			{
				bResult = true;
			}
			else
			{
				CDaemonService::e_LogEvent(EVENTLOG_ERROR_TYPE, "Delete Service Fails");
			}
			::CloseServiceHandle(hService);
		}

		::CloseServiceHandle(hSCM);
		return bResult;
	}

	bool CDaemonService::e_ParseStandardArgs(int argc, char* argv[])
	{
		/// 如果参数个数小于1，可能为系统调用返回false
		if(1 >= argc)
		{
			return false;
		}

		/// 查看服务是否已安装
		if(0 == strcmp(argv[1], "-e"))
		{
			printf("The service is %s installed\n", e_IsInstalled() ? "currently" : "not");
			return true;
		} 
		else if(0 == strcmp(argv[1], "-i"))
		{
			/// 验证是否已经安装了该服务
			if(true == e_IsInstalled())
			{
				printf("%s is already installed\n", m_strServiceName.c_str());
			}
			else
			{
				/// 安装服务
				if(true == e_Install())
				{
					printf("%s installed\n", m_strServiceName.c_str());
				}
				else
				{
					/// 安装服务失败
					printf("%s failed to install. Error %d\n", m_strServiceName.c_str(), GetLastError());
				}
			}
			return true;
		}
		else if(0 == strcmp(argv[1], "-u"))
		{
			/// 验证服务是否存在
			if(false == e_IsInstalled())
			{
				printf("%s is not installed\n", m_strServiceName.c_str());
			}
			else
			{
				/// 卸载服务
				if (true == e_Uninstall())
				{
					TCHAR szFilePath[_MAX_PATH];
					::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
					printf("%s removed. The File %s\r\n", m_strServiceName.c_str(), szFilePath);
				}
				else
				{
					/// 卸载服务失败
					printf("Could not remove %s. Error %d\n", m_strServiceName.c_str(), GetLastError());
				}
			}
			return true;
		}
		/// 未知的命令返回
		return false;
	}

	void CDaemonService::e_IniConfig(const char* pszServiceName, const char* pszServiceDis, CRWIniFile* pRWIniFile)
	{
		if (NULL != pszServiceName)
		{
			m_strServiceName = pszServiceName;
		}

		if (NULL != pszServiceDis)
		{
			m_strServiceDis = pszServiceDis;
		}
		m_pRWIniFile = pRWIniFile;
	}
}