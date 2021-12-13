#
# file:			multibackend_http_server.pro
# path:			workspaces/prj/core/coreapps_qt/multibackend_http_server.pro
# created on:		2021 Dec 13
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#

TEMPLATE = app

include($${PWD}/../../common/common_qt/sys_common.pri)

INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../include/QtHttpServer

DEFINES += QTHTTPSERVERHANDLER_USING_STATIC_LIB_OR_OBJECTS

QT += network core-private
QT += concurrent

LIBS += -lQtHttpServer
LIBS += -lQtSslServer
LIBS += -L$${PRJ_PWD}/$${SYSTEM_PATH}/lib


# HEADERS	 +=  $$files($${repositoryRoot}/include/*.h,true)
# HEADERS	 +=  $$files($${repositoryRoot}/include/*.hpp,true)
# HEADERS	 +=  $${repositoryRoot}/include/QtHttpServer/QtHttpServer
HEADERS	 +=  $$files($${repositoryRoot}/include/qthttpserverhandler/*.h,true)
HEADERS	 +=  $$files($${repositoryRoot}/src/core/multibackend_http_server/*.h,true)
HEADERS	 +=  $$files($${repositoryRoot}/src/core/multibackend_http_server/*.hpp,true)

SOURCES	 +=  $$files($${repositoryRoot}/src/core/multibackend_http_server/*.cpp,true)


#CONFIG += cmdline
