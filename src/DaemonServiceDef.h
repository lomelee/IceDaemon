////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @par 模块名
/// 相关定义模块
/// @par 相关文件
/// 
/// @par 功能详细描述
/// @par 多线程安全性
/// [否，说明]
/// @par 异常时安全性
/// [否，说明]
/// @note         -
/// @file         DaemonServiceDef.h
/// @brief        -
/// @author       Li.xl
/// @version      1.0
/// @date         2018/01/15
/// @todo         -
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ICE_DAEMON_DEAMON_SERVICE_DEF_H_H__
#define __ICE_DAEMON_DEAMON_SERVICE_DEF_H_H__

namespace ICEDAEMON
{	
	// 配置文件名称
	#define CONFIG_FILE_NAME	"./daemon.ini"

	// 服务模块关键字
	#define SERVICE_MOUDLE_KEY	"ServiceDaemon"

	// 服务注册名称
	#define SERVICE_NAME_KEY	"ServiceName"
	// 服务注册描述
	#define SERVICE_DISPLAY		"ServiceDis"

	// 服务守护的可执行文件名
	#define SERVICE_EXEC_NAME	"ServiceExecName"
	// 服务守护的可执行文件路径
	#define SERVICE_EXEC_PATH	"ServiceExecPath"
	// 可执行文件执行的参数
	#define SERVICE_EXEC_ARGV	"ServiceExecArgv"
}

#endif // __ICE_DAEMON_DEAMON_SERVICE_DEF_H_H__