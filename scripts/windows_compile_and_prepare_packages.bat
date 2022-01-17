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
cd /D "%scriptDirectory%.raw"
set "rawScriptsDir=%cd%\"

call .\..\prepare_files.bat

call .\_windows_compile_and_prepare_package.bat x64 Debug
rem call .\_windows_compile_and_prepare_package.bat x64 Release




endlocal
