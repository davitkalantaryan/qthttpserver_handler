#
# file:			sslserver.pro
# path:			workspaces/prj/core/core_qt/sslserver.pro
# created on:		2021 Dec 08
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#

TARGET = QtSslServer
TEMPLATE = lib

include($${PWD}/../../common/common_qt/sys_common.pri)

DEFINES += QT_BUILD_SSLSERVER_LIB

qthttpserverRepoRoot = $${PWD}/../../../qthttpserver
sslSourcesDir = $${qthttpserverRepoRoot}/src/sslserver

#INCLUDEPATH += $${PWD}/../../../include
#INCLUDEPATH += $${PWD}/../../../include_p

QT = network core

HEADERS += \
    $${sslSourcesDir}/qsslserver.h \
    $${sslSourcesDir}/qtsslserverglobal.h \
    $${sslSourcesDir}/qsslserver_p.h

SOURCES += \
    $${sslSourcesDir}/qsslserver.cpp
