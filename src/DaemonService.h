////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 服务模块头文件
/// @par 相关文件
/// CDaemonService.cpp
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         CDaemonService.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ICE_DAEMON_DEAMON_SERVICE_H_H__
#define __ICE_DAEMON_DEAMON_SERVICE_H_H__

#define SVC_ERROR		((DWORD)0xC0020001L)

#include "Common/CommonDef.h"
#include "RWIniFile.h"
#include "ProcessMonitor.h"

namespace ICEDAEMON
{
	class CDaemonService
	{
	public:
		CDaemonService();
		~CDaemonService();

		/// 
		/// @par 功能 
		/// 服务入口函数
		/// @param
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		static void WINAPI e_ServiceMain(DWORD dwArgc, LPSTR* lpszArgv);

		/// 
		/// @par 功能 
		/// 验证服务是否已经安装
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_IsInstalled();

		/// 
		/// @par 功能 
		/// 安装服务程序
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_Install();

		/// 
		/// @par 功能 
		/// 卸载服务
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_Uninstall();

		/// 
		/// @par 功能 
		/// 解析服务安装和卸载命令
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		argc			参数个数
		/// [in]		argv			参数内容
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_ParseStandardArgs(int argc, char* argv[]);

		/// 
		/// @par 功能 
		/// 收到服务管理器消息
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		dwCtrl			消息信息
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		static void WINAPI e_SvcCtrlHandler(DWORD dwCtrl);

		/// 
		/// @par 功能 
		/// 写Windows 日志
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		wType			日志类型
		/// [in]		pszFormat		日志内容
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		static void e_LogEvent(WORD wType, const char* pszFormat, ...);

		/// 
		/// @par 功能 
		/// 上报服务状态,服务状态和SCM交互
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		dwCurrentState	当前状态
		/// [in]		dwWin32ExitCode
		/// [in]		dwWaitHint		等待时间
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		///
		void e_ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

		/// 
		/// @par 功能 
		/// 服务主程序
		/// @param 
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		bool e_ServerProgram(DWORD dwArgc, LPSTR* lpszArgv);

		/// 
		/// @par 功能 
		/// 开始运行服务
		/// @return 	-
		/// @note 		-
		/// @par 示例：
		/// @code 		-
		/// @endcode 
		/// @see		-
		/// @deprecated	- 
		/// 
		void e_ServiceRun();

		/// ==============================================
		/// @par 功能 
		/// 设置配置文件引用
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		void e_IniConfig(const char* pszServiceName, const char* pszServiceDis, CRWIniFile* pRWIniFile);

	public:
		/// 服务状态
		SERVICE_STATUS          m_SvcStatus;
		/// 服务状态句柄
		SERVICE_STATUS_HANDLE   m_SvcStatusHandle;
		/// 服务停止句柄
		HANDLE                  m_hSvcStopEvent;
		// 配置文件对象指针
		CRWIniFile*				m_pRWIniFile;
		// 服务名称
		string					m_strServiceName;
		// 服务描述
		string					m_strServiceDis;
	};
}

#endif	/// __ICE_DAEMON_DEAMON_SERVICE_H_H__