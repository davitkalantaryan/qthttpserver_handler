::
:: File:			_windows_compile_gui.bat
:: Created on:		2021 Aug 07
:: Autor:			Davit Kalantaryan (davit.kalantaryan@gmail.com)
::
:: Purpose:	
::
:: Argumets: 
::
:: Notice: call this from developers command prompt
::

@echo off

setlocal EnableDelayedExpansion enableextensions

set "Platform=%1"
set "Configuration=%2"

set  scriptDirectory=%~dp0
set  currentDirectory=%cd%
cd /D "%scriptDirectory%..\.."
set repositoryRoot=%cd%\
echo "repositoryRoot="%repositoryRoot%


set "Platform=%1"

if /i "%2"=="Debug" (
	set "CONFIG_ADD=CONFIG-=release CONFIG+=debug"
	set "Configuration=Debug"
	set "DirToDelete=debug"
) else (
	set "CONFIG_ADD=CONFIG+=release CONFIG-=debug"
	set "Configuration=Release"
	set "DirToDelete=release"
)


if /i "!VSCMD_ARG_TGT_ARCH!"=="!Platform!" (
	echo "VsDevCmd already set to !Platform!"
) else (
	echo "calling VsDevCmd in the !scriptDirectory!!scriptName!"
	if /i "!Platform!"=="x86" (
		set PlatformTarget=x86
	) else (
		set PlatformTarget=x86_!Platform!
	)
	rem call VsDevCmd -arch=!PlatformTarget!
	cd /D "%VCINSTALLDIR%Auxiliary\Build"
	call vcvarsall.bat !PlatformTarget!
	cd /D %repositoryRoot%
)

rem here we have qt root defination
if not defined QTDIR (
	if not defined qtRoot (
		set "qtRoot=C:\Qt3\5.15.2"
	)
	if /i "%Platform%" == "x64" (
	    set "QTDIR=!qtRoot!\msvc2019_64"
	) else (
	    set "QTDIR=!qtRoot!\msvc2019"
	)
)
echo QTDIR is !QTDIR!

cd "%repositoryRoot%prj\core\corelibs_qt"
if exist "Makefile.*" del /s /q "Makefile.*"
if exist ".qmake.stash" del /s /q ".qmake.stash"
if exist "!DirToDelete!" rmdir /s /q "!DirToDelete!"


call %QTDIR%\bin\qmake sslserver.pro !CONFIG_ADD!
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)
call nmake -f Makefile.QtSslServer.win_%1.!Configuration!
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)

call %QTDIR%\bin\qmake httpserver.pro !CONFIG_ADD!
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)
call nmake -f Makefile.QtHttpServer.win_%1.!Configuration!
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)


exit /b %ERRORLEVEL%

endlocal

::ENDLOCAL & (
::	set additionalPathUsedForDoocs=%additionalPath%
::	set MATLABPATH=%scriptDirectory%%sysAndArch%\mbin;%MATLABPATH%
::	set PYTHONPATH=%scriptDirectory%%sysAndArch%\pbin;%PYTHONPATH%
::	if not defined ENSHOST (
::		set ENSHOST=%defaultEnsHost%
::	)
::)
