///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 读写Ini 配置文件
/// @par 相关文件
/// RWIniFile.cpp
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         RWIniFile.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ICE_DAEMON_RWFINIILE_H_H__
#define __ICE_DAEMON_RWFINIILE_H_H__

#include "Common/CommTypes.h"

class CRWIniFile
{
public:
	// 默认构造函数
	CRWIniFile();

	// 默认析构函数
	virtual ~CRWIniFile();

	/// ==============================================
	/// @par 功能 
	/// 重写构造函数
	/// @param 
	/// [in,out]	<参数名>		<参数说明>
	///	[in]		pszFilePath		配置文件路径	
	/// @return 	-
	/// @note 		Creat By li.xl 2018/01/15 
	/// ==============================================
	CRWIniFile(const char* pszFilePath);

	/// ==============================================
	/// @par 功能 
	/// 初始化文件要读取配置文件路径
	/// @param 
	/// [in,out]	<参数名>		<参数说明>
	///	[in]		pszFilePath		配置文件路径
	/// @return 	-
	/// @note 		Creat By li.xl 2018/01/15 
	/// ==============================================
	bool e_InitRWFile(const char* pszFilePath);

	/// ==============================================
	/// @par 功能 
	/// 写入配置文件
	/// @param 
	/// [in,out]	<参数名>		<参数说明>
	///	[in]		pszConfigKey	配置关键字
	/// [in]		pszConfigName	配置名称
	/// [in]		pszConfigValue	配置值
	/// @return 	-
	/// @note 		Creat By li.xl 2018/01/15 
	/// ==============================================
	bool e_WriteConfigIntoFile(const char* pszConfigKey, const char* pszConfigName, const char* pszConfigValue);

	/// ==============================================
	/// @par 功能 
	/// 读取文字文件关键字
	/// @param 
	/// [in,out]	<参数名>		<参数说明>
	///	[in]		pszConfigKey	配置关键字
	/// [in]		pszConfigName	配置名称
	/// @return 	-
	/// @note 		Creat By li.xl 2018/01/15 
	/// ==============================================
	string e_ReadConfigFromFile(const char* pszConfigKey, const char* pszConfigName);

private:
	// 定义文件路径
	string		m_strFilePath;

};

#endif // __ICE_DAEMON_RWFINIILE_H_H__

