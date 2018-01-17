////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 进程监控相关信息
/// @par 相关文件
/// CProcessMonitor.cpp
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         CProcessMonitor.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ICE_DAEMON_PROCESS_MONITOR__H_H__
#define __ICE_DAEMON_PROCESS_MONITOR__H_H__

#include "Common/CommTypes.h"

namespace ICEDAEMON
{
	class CProcessMonitor
	{
	public:
		CProcessMonitor();
		~CProcessMonitor();

		/// ==============================================
		/// @par 功能 
		/// 启动进程监听守护
		/// @param	
		/// [in,out]	<参数名>			<参数说明>
		///	[in]		pszProcessName		进程名称
		/// [in]		pszProcessPath		进程路径
		/// [in]		pszProcessArgv		进程参数
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool e_StartMonitor(const char* pszProcessName, const char* pszProcessPath, const char* pszProcessArgv);

		/// ==============================================
		/// @par 功能 
		/// 停止进程运行
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool e_StopProcessRun();

	private:

		/// ==============================================
		/// @par 功能 
		/// 通过进程ID获取进程句柄
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		ulProcessID		进程ID
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool i_CheckProcessHasExitByID(ULONG ulProcessID);

		/// ==============================================
		/// @par 功能 
		/// 获取进程句柄ByID
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/17 
		/// ==============================================
		HANDLE i_GetProcessHandleByID(ULONG ulProcessID);

		/// ==============================================
		/// @par 功能 
		/// 启动进程
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool i_StartProcessRun();

		/// ==============================================
		/// @par 功能 
		/// 开启监听线程
		/// @param 
		/// [in,out]	<参数名>		<参数说明>		
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool i_StartMonitorProcessThread();

		/// ==============================================
		/// @par 功能 
		/// 
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	[in]		lpParama		线程参数
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		UINT static WINAPI i_MonitorProcessThread(LPVOID lpParama);

		/// ==============================================
		/// @par 功能 
		/// 
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool i_LoopCheckProcessExsit();

		/// ==============================================
		/// @par 功能 
		/// 
		/// @param 
		/// [in,out]	<参数名>		<参数说明>
		///	
		/// @return 	-
		/// @note 		Creat By li.xl 2018/01/15 
		/// ==============================================
		bool i_RunProcess();

	private:
		// 进程名称
		string				m_strProcessName;
		// 进程路径
		string				m_strProcessPath;
		// 启动参数
		string				m_strProcessArgv;
		// 查询线程句柄
		HANDLE				m_hThreadHandle;
		// 查询线程状态
		HANDLE				m_hThreadStatus;
		// 进程ID
		ULONG				m_ulProcessID;

	};
}
#endif	/// __ICE_DAEMON_PROCESS_MONITOR__H_H__