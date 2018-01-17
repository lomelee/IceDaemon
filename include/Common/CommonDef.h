////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 公共结构或函数定义模块
/// @par 相关文件
/// 引用此文件头的所有文件
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         CommonDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2014/03/17
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __COMM_LDEF_H__
#define __COMM_LDEF_H__

#include "Common/CommTypes.h"

/// 
/// @par 功能 
/// time_t转换为Data
/// @param [in,out]	tTime	time_t时间
///	[in]		tTime			
/// @return 	DATE			DATE时间
/// @note -
/// @par 示例：
/// @code
/// @deprecated -
/// 
static DATE e_TimeToDate(time_t tTime)
{
	DATE dDate = (tTime + 28800) / (24.0 * 60.0 * 60.0) + 25569;
	return dDate;
};

/// 
/// @par 功能 
/// <Data转换为time_t>
/// @param 
///	[in,out]	<参数名>		<参数说明>
///	[in]		DATE			DATE时间
/// @return 	time_t			time_t时间
/// @note -
/// @par 示例：
/// @code
/// @deprecated -
/// 
static time_t e_DateToTime(DATE dDate)
{
	time_t tTime = (time_t)((dDate - 25569) * (24 * 60 * 60) - 28800);
	return tTime;
};

/// 
/// @par 功能 
/// 通过年月日获取该日是星期中第几天
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// [in]		nYear			年
/// [in]		nMonth			月
/// [in]		nDay			日
/// @return 	int				返回星期几
/// @note 
/// <函数说明>
/// @par 示例：
/// @code 
/// <调用本函数的示例代码>
/// @endcode 
/// @see		<参见内容> 
/// @deprecated	<过时说明> 
/// 
static int e_GetWeekDay(int nYear, int nMonth, int nDay)
{
	/*int ny = nYear;
	int nm = nMonth;
	int nd = nDay;
	int nc = 0;
	int nw = 0;
	int weekday = 0;
	char ch = 0;

	if(1 == nm || 2 == nm)
	{
		ny = ny - 1;
		nc = (int)( ny / 100 );
		if( nm == 1 )
		{ nm = 13; }
		else
		{ nm = 14; }
		ny = ny - nc * 100;
	}
	else
	{
		nc = (int) ( ny / 100 );
		ny = ny - nc * 100;
	}

	nw = (int)(nc / 4) - 2 * nc + ny + (int)(ny / 4) + (int)(26 * (nm + 1) / 10) + nd - 1;
	weekday = nw % 7;
	return weekday;*/

	if(nMonth == 1|| nMonth == 2) 
	{
		nMonth += 12;
		nYear--;
	}
	int nWeek = ( nDay + 2 * nMonth + 3 * (nMonth + 1) / 5 + nYear + nYear / 4 - nYear / 100 + nYear / 400) % 7;
	return nWeek;
}

/// 
/// @par 功能 
/// 通过年月日获取该日是该年中第几天
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// [in]		nYear			年
/// [in]		nMonth			月
/// [in]		nDay			日
/// @return 	int				一年中的第几天
/// @note 
/// <函数说明>
/// @par 示例：
/// @code 
/// <调用本函数的示例代码>
/// @endcode 
/// @see		<参见内容> 
/// @deprecated	<过时说明> 
/// 
static int e_GetYearDay(int nYear, int nMonth, int nDay)
{
	int nSum = 0;
	int nLeap = 0;

	/// 先计算某月以前月份的总天数
	switch(nMonth)
	{
	case 1: nSum = 0; break;
	case 2: nSum = 31; break;
	case 3: nSum = 59; break;
	case 4: nSum = 90; break;
	case 5: nSum = 120; break;
	case 6: nSum = 151; break;
	case 7: nSum = 181; break;
	case 8: nSum = 212; break;
	case 9: nSum = 243; break;
	case 10: nSum = 273; break;
	case 11: nSum = 304; break;
	case 12: nSum = 334; break;
	}
	/// 再加上某天的天数
	nSum = nSum + nDay;
	/// 判断是不是闰年
	if(nYear % 400 == 0 
		|| ( nYear % 4 == 0 && nYear % 100 != 0 ))
	{
		nLeap = 1;
	}
	else
	{
		nLeap = 0;
	}
	/// 如果是闰年且月份大于2,总天数应该加一天
	if( 1 == nLeap && nMonth > 2)
	{
		nSum++;
	}

	return nSum;
}

/// 
/// @par 功能 
/// 将格式为[年年年年 月月 日日 时时 分分 秒秒]的字符串转换为日期格式, 
/// 中空格可以缓冲任意个字符
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// [in]		pTime			日期时间字符串
/// @return 	time_t			返回time_t时间
/// @note 
/// <函数说明>
/// @par 示例：
/// @code 
/// <调用本函数的示例代码>
/// @endcode 
/// @see		<参见内容> 
/// @deprecated	<过时说明> 
/// 
static time_t e_StringToTime(const char* pTime)
{
	tm InTimetm;
	time_t InTime = 0;
	char szInYear[10];
	char szInMon[10];
	char szInDay[10];
	char szInHour[10];
	char szInMunite[10];
	char szInSec[10];

	if(NULL == pTime)
	{
		return InTime;
	}

	memset(szInYear, 0x0, sizeof(szInYear));
	memset(szInMon, 0x0, sizeof(szInMon));
	memset(szInDay, 0x0, sizeof(szInDay));
	memset(szInHour, 0x0, sizeof(szInHour));
	memset(szInMunite, 0x0, sizeof(szInMunite));
	memset(szInSec, 0x0, sizeof(szInSec));
	string strAll = pTime;
	if (0 == strAll.length())
	{
		return InTime;
	}
	string strTmp = "";
	strTmp = strAll.substr(0, 4);
	snprintf(szInYear, sizeof(szInYear), "%s", strTmp.c_str());

	strTmp = strAll.substr(5, 2);
	snprintf(szInMon, sizeof(szInMon), "%s", strTmp.c_str());

	strTmp = strAll.substr(8, 2);
	snprintf(szInDay, sizeof(szInDay), "%s", strTmp.c_str());

	strTmp = strAll.substr(11, 2);
	snprintf(szInHour, sizeof(szInHour), "%s", strTmp.c_str());

	strTmp = strAll.substr(14, 2);
	snprintf(szInMunite, sizeof(szInMunite), "%s", strTmp.c_str());

	strTmp = strAll.substr(17, 2);
	snprintf(szInSec, sizeof(szInSec), "%s", strTmp.c_str());

	memset(&InTimetm, 0x0, sizeof(InTimetm));
	InTimetm.tm_year = atoi(szInYear) - 1900;
	InTimetm.tm_mon = atoi(szInMon) - 1;
	InTimetm.tm_mday = atoi(szInDay);
	InTimetm.tm_hour = atoi(szInHour);
	InTimetm.tm_min = atoi(szInMunite);
	InTimetm.tm_sec = atoi(szInSec);
	/// 星期
	InTimetm.tm_wday = e_GetWeekDay(InTimetm.tm_year+1900,InTimetm.tm_mon+1,InTimetm.tm_mday);
	/// 年天
	InTimetm.tm_yday = e_GetYearDay(InTimetm.tm_year+1900,InTimetm.tm_mon+1,InTimetm.tm_mday);
	InTimetm.tm_isdst = 0;
	InTime = mktime(&InTimetm);
	return InTime;
}

static string e_FormatTimeString(const char* pTimeStr)
{
	char szInYear[10];
	char szInMon[10];
	char szInDay[10];
	char szInHour[10];
	char szInMunite[10];
	char szInSec[10];

	memset(szInYear, 0x0, sizeof(szInYear));
	memset(szInMon, 0x0, sizeof(szInMon));
	memset(szInDay, 0x0, sizeof(szInDay));
	memset(szInHour, 0x0, sizeof(szInHour));
	memset(szInMunite, 0x0, sizeof(szInMunite));
	memset(szInSec, 0x0, sizeof(szInSec));

	string strAll = pTimeStr;
	string strTmp = "";
	strTmp = strAll.substr(0, 4);
	snprintf(szInYear, sizeof(szInYear), "%s-", strTmp.c_str());

	strTmp = strAll.substr(4, 2);
	snprintf(szInMon, sizeof(szInMon), "%s-", strTmp.c_str());

	strTmp = strAll.substr(6, 2);
	snprintf(szInDay, sizeof(szInDay), "%s ", strTmp.c_str());

	strTmp = strAll.substr(8, 2);
	snprintf(szInHour, sizeof(szInHour), "%s:", strTmp.c_str());

	strTmp = strAll.substr(10, 2);
	snprintf(szInMunite, sizeof(szInMunite), "%s:", strTmp.c_str());

	strTmp = strAll.substr(12, 2);
	snprintf(szInSec, sizeof(szInSec), "%s", strTmp.c_str());

	strAll.clear();
	strAll.append(szInYear);
	strAll.append(szInMon);
	strAll.append(szInDay);
	strAll.append(szInHour);
	strAll.append(szInMunite);
	strAll.append(szInSec);

	return strAll;
}

/// 
/// @par 功能 
/// 获取tm 时间结构体
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// 
/// @return 		-
/// @note -
/// @par 示例：
/// @code
/// @deprecated -
/// 
static void e_GetTmStruct(tm* m_tm, time_t& t_time)
{
#ifdef WIN32
	localtime_s(m_tm, &t_time);
#else
	tm* ttm = NULL;

	ttm = localtime(&t_time);

	if(NULL != ttm)
	{
		memcpy(m_tm, ttm, sizeof(tm));
	}
#endif
};
/// 
/// @par 功能 
/// 时间类型转换为string第四个参数
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// [in]		t_time			time_t时间
/// [in,out]	pTime			转换后的字符串
/// [in]		nszSize			转换后的字符串大小
/// [in]		flag			要转换成的字符串类型
/// @return 		-
/// @note -
/// @par 示例：
/// @code
/// @deprecated - 
/// 
static void e_TimeToString(time_t t_time, char* pTime, int nszSize, int flag = 4)
{
	int nYear = 0, mon = 0, nDay = 0, hour = 0, min = 0, sec = 0;
	struct tm m_tm;
	e_GetTmStruct(&m_tm, t_time);

	nYear = (int)m_tm.tm_year + 1900;
	mon = (int)m_tm.tm_mon + 1;
	nDay = (int)m_tm.tm_mday;
	hour = (int)m_tm.tm_hour;
	min = (int)m_tm.tm_min;
	sec = (int)m_tm.tm_sec;
	switch(flag)
	{
	case 1:
		{ 
			snprintf(pTime, nszSize, "%04d%02d%02d%02d%02d%02d", nYear, mon, nDay, hour, min, sec); 
			break; 
		}
	case 2:
		{ 
			snprintf(pTime, nszSize, "%04d%02d%02d", nYear, mon, nDay); 
			break; 
		}
	case 3:
		{ 
			snprintf(pTime, nszSize, "%02d%02d%02d", hour, min, sec); 
			break; 
		}
	case 4:
		{ 
			snprintf(pTime, nszSize, "%04d-%02d-%02d %02d:%02d:%02d", nYear, mon, nDay, hour, min, sec); 
			break; 
		}
	case 5:
		{ 
			snprintf(pTime, nszSize, "%04d-%02d-%02d", nYear, mon, nDay); 
			break; 
		}
	case 6:
		{ 
			snprintf(pTime, nszSize, "%nd:%nd:%nd", hour, min, sec); 
			break; 
		}
	case 7:
		{
			hour = (int)t_time / (60 * 60);
			t_time = (int)t_time % ( 60 * 60 );
			min = (int)t_time / 60;
			t_time = (int)t_time % 60;
			sec = (int)t_time;
			snprintf(pTime, nszSize, "%nd:%nd:%nd", hour, min, sec);
			break;
		}
	case 8:
		{
			/// 换算为秒
			hour = (int)t_time / (60 * 60);
			snprintf(pTime, nszSize, "%nd", hour*( 60* 60 )+(min*60)+sec);
			break;
		}
	case 9:
		{
			nYear = nYear % 100;
			/// 换算成期号时间
			snprintf(pTime, nszSize, "%02d%02d%02d000", nYear, mon, nDay);
			break;
		}
	case 10:
		{
			// 转换成指定格式的时间
			snprintf(pTime, nszSize, "%04d/%02d/%02d %02d.%02d.%02d", nYear, mon, nDay, hour, min, sec);
			break; 
		}
	default:
		{
			snprintf(pTime, nszSize, "%04d%02d%02d%02d%02d%02d", nYear, mon, nDay, hour, min, sec);
			break;
		}
	}
};

static void e_GetSysTime(string& strCurrentTime, bool bIsCheckDir = false)
{
	char szTime[64];
	memset(szTime, 0x00, sizeof(szTime));
#ifdef WIN32
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	if(true == bIsCheckDir)
	{
		snprintf(szTime, sizeof(szTime) - 1, "%d-%02d-%02d %02d.%02d.%02d.%03d", 
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,
			SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
	}
	else
	{
		snprintf(szTime, sizeof(szTime) - 1, "%d-%02d-%02d %02d:%02d:%02d.%03d", 
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour,
			SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
	}
	
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ltime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif

	strCurrentTime = szTime;
}

/// 
/// @par 功能 
/// 设置系统时间(设置网络传输处理误差时间都增加了1秒钟)
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// [in]		tmStruct		要设置的tm结构体	
/// @return 	bool			设置系统时间成功或者失败
/// @note -
/// @par 示例：
/// @code
/// @deprecated - 
/// 
static bool e_SetSystemTime(tm& tmStruct)
{
	time_t tTime = mktime(&tmStruct);

#ifdef WIN32

	SYSTEMTIME SystemTime;

	SystemTime.wYear = tmStruct.tm_year + 1900;
	SystemTime.wMonth = tmStruct.tm_mon + 1;
	SystemTime.wDay = tmStruct.tm_mday;
	SystemTime.wHour = tmStruct.tm_hour;
	SystemTime.wMinute = tmStruct.tm_min;
	SystemTime.wSecond = tmStruct.tm_sec + 1;
	SystemTime.wDayOfWeek = tmStruct.tm_wday;
	SystemTime.wMilliseconds = 0;

	BOOL bRet = SetLocalTime(&SystemTime);
	return (bRet == TRUE);

#else

	struct timeval tv;
	tv.tv_sec = tTime + 1;
	tv.tv_usec = 0;
	settimeofday(&tv, 0);
	return true;

#endif
};

/// ==============================================
/// @par 功能 
/// 把秒转换为始终显示
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	[in]		ulSecondsTime	秒数
/// [out]		strSecondsTime	转换后的字符串分钟数据 格式:[HH:mm:ss]
/// @return 	-
/// @note 		Creat By li.xl 2014/06/05 
/// ==============================================
static void e_TransSecondsToStr(ULONG ulSecondsTime, string& strSecondsTime)
{
	int nHour = 0, nMins = 0, nSecond = 0;

	nHour = ulSecondsTime / 3600;

	ulSecondsTime = ulSecondsTime % 3600;

	nMins = ulSecondsTime / 60;

	nSecond = ulSecondsTime % 60;

	char szTempTime[16];
	memset(szTempTime, 0x00, sizeof(szTempTime));
	snprintf(szTempTime, sizeof(szTempTime) - 1, 
	  "%02d:%02d:%02d", nHour, nMins, nSecond);
	strSecondsTime = szTempTime;
}

/// 定义内存换算单位
#define MEMDIV 1024

/// 
/// @par 功能 
/// 获取可用内存
/// @param 
/// [in,out]	<参数名>		<参数说明>
/// @return 	Int				可用内存大小，单位M
/// @note 		-
/// @par 示例：
/// @code 		-
/// @endcode 
/// @see		-
/// @deprecated	- 
/// 
inline int e_GetSystemMemoryStat()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	//cout<<"statex.dwLingth :"<<statex.dwLength<<endl;
	GlobalMemoryStatusEx (&statex);

	int nMemoryAvail = (int)(statex.ullAvailPhys / (MEMDIV * MEMDIV));

	//cout<<"物理内存可用容量："<< nMemoryAvail <<"M"<<endl;
	//cout<<"物理内存总容量："<<statex.ullTotalPhys/MEMDIV/MEMDIV<<"M"<<endl;
	//cout<<"虚拟内存总容量："<<statex.ullTotalVirtual/MEMDIV/MEMDIV<<"M"<<endl;
	//cout<<"虚拟内存可用容量："<<statex.ullAvailVirtual/MEMDIV/MEMDIV<<"M"<<endl;
	return nMemoryAvail;
}

/// 
/// @par 功能 
/// 对比时间片
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	[in]		time1			第一次系统时间片
///	[in]		time2			第二次系统时间片
/// @return 	-
/// @note 		-
/// @par 示例：
/// @code 		-
/// @endcode 
/// @see		-
/// @deprecated	- 
/// 
inline INT64 e_CompareFileTime(FILETIME time1, FILETIME time2)
{
	/// 忽略警告把值强制转化为无符号
	INT64 a = (UINT64)time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	INT64 b = (UINT64)time2.dwHighDateTime << 32 | time2.dwLowDateTime;
	return (b - a);
};

/// 
/// @par 功能 
/// 获取CPU使用率
/// @param 
/// [in,out]	<参数名>		<参数说明>
/// @return 	int				使用率百分比
/// @note 		-
/// @par 示例：
/// @code 		-
/// @endcode 
/// @see		-
/// @deprecated	- 
/// 
inline int e_GetSystemCupStat()
{ 
	/// 声明事件信号
	HANDLE hEvent;
	BOOL res;

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	/// 获取系统时间片
	res = GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

	/// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	/// 如果没有获取到信号，等待1500毫秒返回
	WaitForSingleObject(hEvent, 1500);

	/// 再次获取系统时间片
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	INT64 idle	 = e_CompareFileTime(preidleTime, idleTime);
	INT64 kernel = e_CompareFileTime(prekernelTime, kernelTime);
	INT64 user	 = e_CompareFileTime(preuserTime, userTime);

	/// CPU利用率:
	int nUseCpu = (int)((kernel + user - idle) * 100 / (kernel + user));
	///// CPU空闲率:
	//int nCpuIdle = (int)((idle) * 100 / (kernel + user));
	///// 打印
	//printf("CPU利用率: %d%%, 空闲率 %d%%\r\n", nUseCpu, nCpuIdle);
	return nUseCpu;
};

/// 
/// @par 功能 
/// 替换字符串中的字符
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	[in]		str				目标字符串
///	[in]		pattern			要替换的字符/字符串
///	[in]		dstPattern		替换后的字符/字符串
/// @return 	-
/// @note 		-
/// @par 示例：
/// @code 		-
/// @endcode 
/// @see		-
/// @deprecated	- 
/// 
static string e_ReplaceAll(string str, string pattern, string dstPattern)
{
	std::string retStr="";
	string::size_type pos;

	int szStr = str.length();
	int szPattern = pattern.size();
	int i=0;
	int l_count=0;
	// replace all
	int count = szStr;

	for(i=0; i<szStr; i++)
	{
		pos=str.find(pattern,i);

		if(pos == std::string::npos)
		{
			break;
		}
		if(szStr > (int)pos)
		{
			std::string s=str.substr(i,pos-i);
			retStr += s;
			retStr += dstPattern;
			i=pos+pattern.length()-1;
			if(++l_count >= count)
			{
				i++;
				break;
			}
		}
	}
	retStr += str.substr(i);
	return retStr;
}

/// ==============================================
/// @par 功能 
/// 查找字符串中第几次出现目标字符串的位置
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	
/// @return 	-
/// @note 		by li.xl 2013/04/27 
/// ==============================================
inline int e_Instr(char* pszSrc, char* pszDes, USHORT usIndex)
{
	int nFindeIndex = 0;
	/// 验证数据
	if(NULL == pszSrc || NULL == pszDes)
	{
		return nFindeIndex;
	}

	bool bIsFind = false;
	/// 赋值数据源
	char* pszCharTemp = pszSrc;
	while(NULL != (pszCharTemp = strstr(pszCharTemp, pszDes)))
	{
		nFindeIndex++;
		if(nFindeIndex == usIndex)
		{
			bIsFind = true;
			break;
		}
		// 移动查找到的位置，避免下次查找依然是此位置
		pszCharTemp = pszCharTemp + strlen(pszDes);
	}
	/// 如果找对应位置的字符串字串
	if(true == bIsFind)
	{
		nFindeIndex = pszCharTemp - pszSrc;
	}
	return nFindeIndex;
}

/// ==============================================
/// @par 功能 
/// 设置当前运行目录
/// @return 	-
/// @note 		by li.xl 2013/04/12 
/// ==============================================
inline void e_SetCurDirectory()
{
	/// 设置当前运行目录
	char szBuf[512];
	char szDirectPath[512];
	memset(szDirectPath, 0x00, sizeof(szDirectPath));
	/// 得到当前模块路径
	::GetModuleFileName(NULL, szDirectPath, sizeof(szDirectPath) - 1);
	char* pszLastPoint = strrchr(szDirectPath, '\\');
	if(NULL != pszLastPoint)
	{
		UINT nSize = pszLastPoint - szDirectPath;
		if(0 < nSize)
		{
			strncpy_s(szBuf, szDirectPath, nSize);
			szBuf[nSize] = 0;
		}
	}
	::SetCurrentDirectory(szBuf);
}

/// ==============================================
/// @par 功能 
/// 创建多级目录
/// @param 
/// [in,out]	<参数名>		<参数说明>
/// @return 	-
/// @note 		by li.xl 2013/04/19 
/// ==============================================
inline bool e_CreatUserDir(char* pszDir)
{
	int i = 0;
	int iRet;
	int iLen = strlen(pszDir);

	// 在末尾加 “/”
	if(pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
	}

	// 创建目录
	for(i = 0; i <= iLen; i++)
	{
		if(pszDir[i] == '\\' || pszDir[i] == '/')
		{ 
			pszDir[i] = '\0';
			// 如果不存在,创建
			if(0 != (iRet = _access(pszDir, 0)))
			{
				if(0 != (iRet = _mkdir(pszDir)))
				{
					return false;
				}
			}
			// 支持linux,将所有\换成/
			pszDir[i] = '/';
		} 
	}
	return true;
};


/// 
/// @par 功能 
/// 获取当前文件运行目录
/// @param 
///	[in,out]	<参数名>		<参数说明>
/// @return 	string			返回文件路径
/// @note 
/// <函数说明>
/// @par 示例：
/// @code 
/// <调用本函数的示例代码>
/// @endcode 
/// @see		<参见内容> 
/// @deprecated	<过时说明> 
/// 
static string e_GetFilePath()
{
	/// 定义获取字符字符串
	char szExePath[512];
	memset(szExePath, 0x00, sizeof(szExePath));
	/// 获取定义的地址长度
	int nPathLen = sizeof(szExePath);
	/// 定义获取返回的变量
	DWORD ldwReturn = 0;
	/// 定义字符指针
	char* pPos = NULL;
	/// 定义路径间隔目录字符
	char cInchar = '\\';

	/// 获取路径
#ifndef WIN32
	ldwReturn = readlink( "/proc/self/exe", szExePath, nPathLen);
	cInchar = '/';
#else
	ldwReturn = GetModuleFileNameA(NULL, szExePath, nPathLen);
	cInchar = '\\';
#endif

	if(0 < (int)ldwReturn && nPathLen > (int)ldwReturn)
	{
		pPos = strrchr(szExePath, cInchar);
		if(NULL != pPos)
		{
			*++pPos = '\0';
		}
	}
	/// 处理返回获取到的路径字符串
	string strExePath = szExePath;
	return strExePath;
}

/// ==============================================
/// @par 功能 
/// 获取一个INI的某一bit位的值
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	
/// @return 	-
/// @note 		Creat By li.xl 2013/12/23 
/// ==============================================
static int e_GetBit(int nTest, int index) 
{
	return ((nTest & (1 << index)) > 0) ? 1: 0;
}

/// ==============================================
/// @par 功能 
/// 获取系统时间
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	
/// @return 	秒(时*3600+分*60+秒)
/// @note 		Creat By li.xl 2014/01/10 
/// ==============================================
static UINT e_GetSysTime()
{
	UINT unTimeRet = 0;
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	unTimeRet = SystemTime.wHour * 3600;
	unTimeRet += SystemTime.wMinute * 60;
	unTimeRet += SystemTime.wSecond;

	return unTimeRet;
}

static bool e_CheckFileExist(const char* pszFilePath)
{
	// 定义返回值
	bool bRet = false;
	// 验证数据合法性
	if(NULL == pszFilePath)
	{
		return bRet;
	}
	// 定义检测文件是否存在返回值
	int nRet = 0;
	/// 检测文件是否存在
#ifdef WIN32
	struct _stat	tFileStatus;
	nRet = _stat(pszFilePath, &tFileStatus);
#else
	struct stat		tFileStatus;
	nRet = stat(pszFilePath, &tFileStatus);
#endif
	bRet = (0 == nRet);
	return bRet;
}

static bool e_MakeStrSub(const char* pszStrSrc, string& strStrDes)
{
	bool bRet = false;
	int nStrLen = 0;
	strStrDes = "";
	char cTemp;
	if(NULL != pszStrSrc && 0 < (nStrLen = strlen(pszStrSrc)))
	{
		for(int i = 0; i < nStrLen; i++)
		{
			cTemp = pszStrSrc[i];
			strStrDes += cTemp;
			strStrDes += " ";
		}
		bRet = true;
	}	
	return bRet;
}

/// ==============================================
/// @par 功能 
/// 获取全路径中的文件名
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	
/// @return 	-
/// @note 		Creat By li.xl 2015/05/10 
/// ==============================================
inline string e_GetNameFromPath(const char* pszFullName)
{
	string strFileName = pszFullName;
	const char*  pszFistPoint = pszFullName;
	const char*  pszLastPoint  = pszFullName + strlen(pszFullName);
	if ( strrchr( pszFullName, '\\' ) != NULL )
		pszFistPoint = strrchr( pszFullName, '\\' ) + 1;
	else if ( strrchr( pszFullName, '/' ) != NULL )
		pszFistPoint = strrchr( pszFullName, '/' ) + 1;
	//if ( strrchr( pszFullName, '.' ) != NULL )
	//	pszLastPoint = strrchr( pszFullName, '.' );
	if ( pszLastPoint < pszFistPoint )
		pszLastPoint = pszFullName + strlen( pszFullName );

	strFileName.assign(pszFistPoint, pszLastPoint);

	return strFileName;
}

/// ==============================================
/// @par 功能 
/// 获取当前线程ID
/// @param 
/// [in,out]	<参数名>		<参数说明>
///	-
/// @return 	当前线程的ID 
/// @note 		Creat By li.xl 2016/08/12 
/// ==============================================
inline UINT e_GetCurrentThreadID()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return thread_self();
#endif
}

#endif	/// __COMM_LDEF_H__