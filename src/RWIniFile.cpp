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

#include "RWIniFile.h"
#include "Common/CommonDef.h"


CRWIniFile::CRWIniFile()
{
	m_strFilePath = "";
}


CRWIniFile::~CRWIniFile()
{
}

CRWIniFile::CRWIniFile(const char* pszFilePath)
{
	m_strFilePath = "";
	e_InitRWFile(pszFilePath);
}


bool CRWIniFile::e_InitRWFile(const char* pszFilePath)
{
	if (NULL == pszFilePath || '\0' == pszFilePath)
	{
		return false;
	}

	if (e_CheckFileExist(pszFilePath))
	{
		m_strFilePath = pszFilePath;
		return true;
	}
	return false;
}

bool CRWIniFile::e_WriteConfigIntoFile(const char* pszConfigKey, const char* pszConfigName, const char* pszConfigValue)
{
	if (NULL == pszConfigKey)
	{
		return false;
	}

	if (NULL == pszConfigName || '\0' == pszConfigName)
	{
		return false;
	}

	if (NULL == pszConfigValue)
	{
		return false;
	}
	return (TRUE == WritePrivateProfileString(pszConfigKey, pszConfigName, pszConfigValue, m_strFilePath.c_str()));
}

string CRWIniFile::e_ReadConfigFromFile(const char* pszConfigKey, const char* pszConfigName)
{
	if (NULL == pszConfigKey)
	{
		return "";
	}

	if (NULL == pszConfigName || '\0' == pszConfigName)
	{
		return "";
	}
	// 定义获取的返回值
	char szConfigValue[256];
	memset(szConfigValue, 0x00, sizeof(szConfigValue));
	// 获取配置文件的
	GetPrivateProfileString(pszConfigKey, pszConfigName, "", szConfigValue, sizeof(szConfigValue) - 1, m_strFilePath.c_str());
	// 返回获取的配置信息
	return szConfigValue;
}