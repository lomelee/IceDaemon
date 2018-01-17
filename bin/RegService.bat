@echo off
choice /C YN /M "您确定安装控制服务吗? 确定请按Y，取消请按N"
if errorlevel 2 goto end1
if errorlevel 1 goto run

:run

%~dp0IceDaemon.exe -i
echo 开始启动服务
net start ICEService
echo 设置服务开机自动启动
sc config ICEService start= auto
::echo 设置服务自动重启间隔时间15s
::sc failure ICEService reset= 2592000 actions= restart/15000
goto end2

:end1
echo 您取消了启动服务,窗口将关闭
exit

:end2
echo 服务已启动完成,按任意键关闭窗口
pause>nul