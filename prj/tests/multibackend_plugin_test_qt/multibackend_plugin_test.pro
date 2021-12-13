#
# file:			multibackend_plugin_test.pro
# path:			workspaces/prj/tests/multibackend_plugin_test/multibackend_plugin_test.pro
# created on:		2021 Dec 13
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#

TEMPLATE = lib

include($${PWD}/../../common/common_qt/sys_common.pri)

INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../include/QtHttpServer

QT += network core-private
QT += concurrent

LIBS += -lQtHttpServer
LIBS += -lQtSslServer
LIBS += -L$${PRJ_PWD}/$${SYSTEM_PATH}/lib

SOURCES += \
    $${repositoryRoot}/src/tests/dllmain_multibackend_plugin_test.cpp

