#
# file:			servtest01.pro
# path:			workspaces/prj/tests/servtest01_qt/servtest01.pro
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
    "$${PWD}/../../../src/tests/main_any_test.cpp"


CONFIG += cmdline
