#
# file:			servtest02.pro
# path:			workspaces/prj/tests/servtest01_qt/servtest02.pro
# created on:		2021 Dec 08
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#

TEMPLATE = app

include($${PWD}/../../common/common_qt/sys_common.pri)

INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../include/QtHttpServer

qthttpserverRepoRoot = $${PWD}/../../../qthttpserver

QT += network core-private
QT += concurrent

LIBS += -lQtHttpServer
LIBS += -lQtSslServer
LIBS += -L$${PRJ_PWD}/$${SYSTEM_PATH}/lib

SOURCES += \
    $${qthttpserverRepoRoot}/examples/httpserver/afterrequest/main.cpp

CONFIG += cmdline
