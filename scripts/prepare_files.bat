@echo off
setlocal EnableDelayedExpansion


set scriptName=%0
set scriptDirectory=%~dp0
set currentDirectory=%cd%


cd /D "%scriptDirectory%.."
set "repositoryRoot=%cd%\"
echo "repositoryRoot="%repositoryRoot%


if not exist include_p\private (mkdir include_p\private)
if not exist include\QtSslServer (mkdir include\QtSslServer)
if not exist include\QtHttpServer (mkdir include\QtHttpServer)
rem QtSslServer
rem QtHttpServer

copy /Y qthttpserver\src\sslserver\qsslserver_p.h include_p\private\.
copy /Y qthttpserver\src\sslserver\qsslserver.h include\QtSslServer\.
copy /Y qthttpserver\src\sslserver\qtsslserverglobal.h include\QtSslServer\.

copy /Y qthttpserver\src\httpserver\*.h include\QtHttpServer\.
copy /Y qthttpserver\src\httpserver\*_p.h include_p\private\.
del /Q include\QtHttpServer\*_p.h


cd include\QtHttpServer

echo #pragma once > QtHttpServer

for %%I in ("*.hpp" "*.h") do (
	set "relFilePath=%1\%%~nI.$(Platform).$(ObjectsExtension).obj"
	set newLine=#include "%%I"
	echo !newLine! >> QtHttpServer
)



endlocal
::endlocal & (
::	if defined VsDevCmd_version_defined_here (
::		set VsDevCmd_version=!VsDevCmd_version!
::	)
::)
