::
:: File:			windows_compile_and_prepare_64bit_release_package.bat
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

set  scriptDirectory=%~dp0
set  currentDirectory=%cd%
cd /D "%scriptDirectory%.."
set "repositoryRoot=%cd%\"

set "Branch=0"

for %%x in (%*) do (
	set "Branch=%%x"
	echo Branch is: !Branch!
)

if not "!Branch!"=="0" (
	git fetch -p origin
	git checkout !Branch!
	git pull origin !Branch!
	git submodule sync --recursive
	git submodule update --init --recursive
)

if exist "%repositoryRoot%sys\win_x64\Release\.objects" rmdir /q /s "%repositoryRoot%sys\win_x64\Release\.objects"
if exist "%repositoryRoot%sys\win_x64\Release\bin" rmdir /q /s "%repositoryRoot%sys\win_x64\Release\bin"
if exist "%repositoryRoot%deploy" rmdir /q /s "%repositoryRoot%deploy"

cd /D "%scriptDirectory%"

if /i "!VSCMD_ARG_TGT_ARCH!"=="x64" (
	echo "vcvarsall.bat already set to x64"
) else (
	echo "calling vcvarsall.bat in the !scriptDirectory!!scriptName!"
	cd /D "%VCINSTALLDIR%Auxiliary\Build"
	call vcvarsall.bat x86_x64
	cd /D %scriptDirectory%
)

rem here we have qt root defination
if not defined QTDIR (
	if not defined qtRoot (
		set "qtRoot=C:\Qt3\5.15.2"
	)
	set "QTDIR=!qtRoot!\msvc2019_64"
)
echo QTDIR is !QTDIR!

call .\.raw\_windows_compile_httpserver.bat x64 Release
if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
call .\.raw\_windows_prepare_package.bat x64 Release
exit /b %ERRORLEVEL%

endlocal
