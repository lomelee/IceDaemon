@echo off
net stop ICEService
%~dp0IceDaemon.exe -u

echo 服务已卸载完成,按任意键关闭窗口
pause>nul
