////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 进程监控相关信息
/// @par 相关文件
/// CProcessMonitor.h
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         CProcessMonitor.cpp
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Common/CommTypes.h"
#include "psapi.h"
#include "tlhelp32.h" 
#include "stdio.h"
#include "ProcessMonitor.h"

namespace ICEDAEMON
{
	CProcessMonitor::CProcessMonitor()
	{
		// 进程名
		m_strProcessName = "";
		m_strProcessPath = "";
		// 启动进程参数
		m_strProcessArgv = "";
		// 线程状态信号
		m_hThreadStatus = CreateEvent(NULL, FALSE, FALSE, NULL);
		// 初始化线程句柄
		m_hThreadHandle = NULL;
		// 进程ID
		m_ulProcessID = 0;
	}

	CProcessMonitor::~CProcessMonitor()
	{
	}

	bool CProcessMonitor::e_StartMonitor(const char* pszProcessName, const char* pszProcessPath, const char* pszProcessArgv)
	{
		if (NULL == pszProcessName || NULL == pszProcessPath || NULL == pszProcessArgv)
		{
			return false;
		}
		// 进程路径
		m_strProcessName = pszProcessName;
		// 构造进程全路径
		m_strProcessPath = pszProcessPath;
		if (0 < m_strProcessPath.length())
		{
			if ('/' != m_strProcessPath.at(m_strProcessPath.length() - 1) && 
				'\\' != m_strProcessPath.at(m_strProcessPath.length() - 1))
			{
				m_strProcessPath.append("/");
			}
			m_strProcessPath.append(pszProcessName);
		}		
		m_strProcessArgv = pszProcessArgv;
		// 开始监听进程
		i_StartMonitorProcessThread();
		return true;
	}

	bool CProcessMonitor::i_StartMonitorProcessThread()
	{
		bool bRet = false;
		// 如果线程不存在
		if(NULL == m_hThreadHandle)
		{
			// 启动线程
			m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, 
				i_MonitorProcessThread, this, 0, NULL);
		}

		if(NULL != m_hThreadHandle)
		{
			bRet = true;
		}
		return bRet;
	}

	UINT CProcessMonitor::i_MonitorProcessThread(LPVOID lpParama)
	{
		CProcessMonitor* pThis = (CProcessMonitor*) lpParama;
		if (NULL != pThis)
		{
			pThis->i_LoopCheckProcessExsit();
		}
		return 0;
	}


	bool CProcessMonitor::i_LoopCheckProcessExsit()
	{
		bool bIsFindProcess = false;
		// 开始线程循环
		while(true)
		{
			bIsFindProcess = false;
			// 如果存在进程ID
			if (0 < m_ulProcessID)
			{
				bIsFindProcess = i_CheckProcessHasExitByID(m_ulProcessID);
			}			
			// 如果没有找到进程，通过名称获取
			if (false == bIsFindProcess)
			{
				HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				PROCESSENTRY32 pe32;
				pe32.dwSize = sizeof(pe32);
				BOOL IsCanGetMore = FALSE;
				do 
				{
					IsCanGetMore = ::Process32Next(hProcessSnap, &pe32);
					// 找到了相应的进程名，退出查找，等待一下次查找
					if(TRUE == IsCanGetMore && 0 == strcmp(m_strProcessName.c_str(), pe32.szExeFile))
					{
						// 赋值找到了指定的进程名
						bIsFindProcess =  true;
						break;
					}
				} while (TRUE == IsCanGetMore);

				// 如果没有找到指定的进程
				if (false == bIsFindProcess)
				{
					// 启动进程
					i_StartProcessRun();
				}
			}

			// 如果存在信号，退出处理线程 30秒 处理一次查询
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_hThreadStatus, 10000))
			{
				break;
			}
		}

		return false;
	}

	/// 通过进程ID获取进程句柄
	bool CProcessMonitor::i_CheckProcessHasExitByID(ULONG ulProcessID)
	{
		bool bIsExsit = false;
		
		HANDLE processHandle = i_GetProcessHandleByID(ulProcessID);
		if (NULL != processHandle)
		{
			// 释放句柄引用计数
			CloseHandle(processHandle);
			// 赋值存在返回值
			bIsExsit = true;
		}
		return bIsExsit;
	}

	HANDLE CProcessMonitor::i_GetProcessHandleByID(ULONG ulProcessID)
	{
		if (0 < ulProcessID)
		{
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ulProcessID);
		}
		return NULL;
	}

	bool CProcessMonitor::i_StartProcessRun()
	{
		//进程信息
		PROCESS_INFORMATION pro_info;
		//启动信息
		STARTUPINFO sti; 
		// 初始化两个结构体  
		ZeroMemory(&pro_info, sizeof(PROCESS_INFORMATION));
		ZeroMemory(&sti, sizeof(STARTUPINFO));
		// 构造进程启动命令
		char szCmdLine[256];
		memset(szCmdLine, 0x00, sizeof(szCmdLine) - 1);
		snprintf(szCmdLine, sizeof(szCmdLine) - 1, "\"%s\" %s", m_strProcessPath.c_str(), m_strProcessArgv.c_str());
		// 赋值进程ID
		// CreateProcessAsUser(创建一个GUI进程)
		// 创建一个静默的系统进程
		if(TRUE == ::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &sti, &pro_info))
		{
			m_ulProcessID = pro_info.dwProcessId;
			// 释放线程和句柄的引用计数
			CloseHandle(pro_info.hProcess);
			CloseHandle(pro_info.hThread);
			return true;
		}
		return false;
	}

	bool CProcessMonitor::e_StopProcessRun()
	{
		HANDLE hProcessHandle = i_GetProcessHandleByID(m_ulProcessID);
		if (NULL != hProcessHandle)
		{
			/// 退出码
			DWORD exitCode;
			/// 获取退出码  
			GetExitCodeProcess(hProcessHandle, &exitCode);
			TerminateProcess(hProcessHandle, exitCode);  
			// 释放句柄引用计数
			CloseHandle(hProcessHandle);
			return true;
		}
		return false;
	}
}