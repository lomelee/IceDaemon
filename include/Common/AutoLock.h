////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// <数据自动锁模块>
/// @par 相关文件
/// CommTypes.h
/// @par 功能详细描述
/// <这里是本功能的详细描述，内容超过120个半角字符需要加反斜杠N换行接上一行内容>
/// @par 多线程安全性
/// <是/否>[否，说明]
/// @par 异常时安全性
/// <是/否>[否，说明]
/// @note         -
/// 
/// @file         AutoLock.h
/// @brief        <模块功能简述>
/// @author       Li.xl
/// @version      1.0
/// @date         2011/05/25
/// @todo         <将来要作的事情>
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __VAPP_LIGHTAUTOLOCK_H__
#define __VAPP_LIGHTAUTOLOCK_H__

#include "CommTypes.h"

////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 	异常类
/// <异常类说明>
/// @par 	备注
/// <为了不允许，异部线程发送数据，给线程加锁>
/// @par 	典型用法
/// @code
/// <这里是典型用法的示例代码>
/// @endcode
/// 
/// @brief	
/// @author	Li.xl
/// 
////////////////////////////////////////////////////////////////////////////////
#ifndef WIN32
/// linux
class CLightCritSec 
{
	/// Linux锁封装
	CLightCritSec(const CLightCritSec &refCritSec);
	CLightCritSec &operator=(const CLightCritSec &refCritSec);

private:
	pthread_mutexattr_t m_attr;
	pthread_mutex_t m_mutex;
public:
	CLightCritSec() 
	{
		pthread_mutexattr_init(&m_attr);
		//pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_TIMED_NP);
		//pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
		//pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ADAPTIVE_NP);
		pthread_mutex_init(&m_mutex, &m_attr);
	};

	~CLightCritSec()
	{
		pthread_mutex_destroy(&m_mutex);
	};

	void e_Lock()
	{
		pthread_mutex_lock(&m_mutex);
	};

	void e_Unlock() 
	{
		pthread_mutex_unlock(&m_mutex);
	};
};

#else 

class CLightCritSec 
{
	/// 
	/// @par 功能 
	/// 重载构造函数
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CLightCritSec(const CLightCritSec &refCritSec);

	/// 
	/// @par 功能 
	/// 重载等号操作符
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CLightCritSec &operator=(const CLightCritSec &refCritSec);

private:
	CRITICAL_SECTION m_CritSec;
public:

	CLightCritSec()
	{
		InitializeCriticalSectionAndSpinCount(&m_CritSec, 10000);
		/// InitializeCriticalSection(&m_CritSec);
	};

	~CLightCritSec()
	{
		DeleteCriticalSection(&m_CritSec);
	};

	/// 
	/// @par 功能 
	/// 加锁
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	void e_Lock()
	{
		EnterCriticalSection(&m_CritSec);
	};

	/// 
	/// @par 功能 
	/// 解锁
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	void e_Unlock()
	{
		LeaveCriticalSection(&m_CritSec);
	};
};
#endif

////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 	异常类
/// <异常类说明>
/// @par 	备注
/// <加锁业务实现类>
/// @par 	典型用法
/// @code
/// <这里是典型用法的示例代码>
/// @endcode
/// 
/// @brief	
/// @author	Li.xl
/// 
////////////////////////////////////////////////////////////////////////////////
class CAutoLock 
{
	/// 
	/// @par 功能 
	/// 重载构造函数
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CAutoLock(const CAutoLock &refAutoLock);

	/// 
	/// @par 功能 
	/// 重载等号操作符
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CAutoLock &operator = (const CAutoLock &refAutoLock);

protected:
	/// 当期锁住的线程
	CLightCritSec* m_pLock;
public:

	/// 
	/// @par 功能 
	/// 构造函数
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CAutoLock()	
	{
		m_pLock = NULL;
	};

	/// 
	/// @par 功能 
	/// 构造函数重载
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// [in]		plock			当前操作的对象
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	CAutoLock(CLightCritSec* plock)
	{
		m_pLock = plock;
		if(NULL != m_pLock)
		{
			m_pLock->e_Lock();
		}
	};

	/// 
	/// @par 功能 
	/// 析构函数,解锁当前操作
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// 
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	~CAutoLock() 
	{
		if(NULL != m_pLock)
		{
			m_pLock->e_Unlock();
		}
	};

	/// 
	/// @par 功能 
	/// 锁住当前操作的连接
	/// @param 
	///	[in,out]	<参数名>		<参数说明>
	/// [in]		plock			当前操作的对象
	/// @return 		-
	/// @note 
	/// <函数说明>
	/// @par 示例：
	/// @code 
	/// <调用本函数的示例代码>
	/// @endcode 
	/// @see		<参见内容> 
	/// @deprecated	<过时说明> 
	/// 
	void e_Lock(CLightCritSec* plock) 
	{
		m_pLock = plock;
		if(NULL != m_pLock)
		{
			m_pLock->e_Lock();
		}
	};
};
#endif/// __VAPP_LIGHTAUTOLOCK_H__