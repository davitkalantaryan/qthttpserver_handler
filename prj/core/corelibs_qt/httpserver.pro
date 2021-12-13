#
# file:			httpserver.pro
# path:			workspaces/prj/core/core_qt/httpserver.pro
# created on:		2021 Dec 08
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#

TARGET = QtHttpServer
TEMPLATE = lib

include($${PWD}/../../common/common_qt/sys_common.pri)

DEFINES += QT_BUILD_HTTPSERVER_LIB

qthttpserverRepoRoot = $${PWD}/../../../qthttpserver
httpSourcesDir = $${qthttpserverRepoRoot}/src/httpserver


INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../include_p

QT = network core-private

qtHaveModule(websockets): QT += websockets-private
#qtConfig(ssl): QT += sslserver

LIBS += -lQtSslServer
LIBS += -L$${PRJ_PWD}/$${SYSTEM_PATH}/lib

HEADERS += \
    $${httpSourcesDir}/qthttpserverglobal.h \
    $${httpSourcesDir}/qabstracthttpserver.h \
    $${httpSourcesDir}/qabstracthttpserver_p.h \
    $${httpSourcesDir}/qhttpserver.h \
    $${httpSourcesDir}/qhttpserver_p.h \
    $${httpSourcesDir}/qhttpserverliterals_p.h \
    $${httpSourcesDir}/qhttpserverrequest.h \
    $${httpSourcesDir}/qhttpserverrequest_p.h \
    $${httpSourcesDir}/qhttpserverresponder.h \
    $${httpSourcesDir}/qhttpserverresponder_p.h \
    $${httpSourcesDir}/qhttpserverresponse.h \
    $${httpSourcesDir}/qhttpserverresponse_p.h \
    $${httpSourcesDir}/qhttpserverrouter.h \
    $${httpSourcesDir}/qhttpserverrouter_p.h \
    $${httpSourcesDir}/qhttpserverrouterrule.h \
    $${httpSourcesDir}/qhttpserverrouterrule_p.h \
    $${httpSourcesDir}/qhttpserverrouterviewtraits.h \
    $${httpSourcesDir}/qhttpserverviewtraits.h \
    $${httpSourcesDir}/qhttpserverviewtraits_impl.h

SOURCES += \
    $${httpSourcesDir}/qabstracthttpserver.cpp \
    $${httpSourcesDir}/qhttpserver.cpp \
    $${httpSourcesDir}/qhttpserverliterals.cpp \
    $${httpSourcesDir}/qhttpserverrequest.cpp \
    $${httpSourcesDir}/qhttpserverresponder.cpp \
    $${httpSourcesDir}/qhttpserverresponse.cpp \
    $${httpSourcesDir}/qhttpserverrouter.cpp \
    $${httpSourcesDir}/qhttpserverrouterrule.cpp

qtHaveModule(concurrent) {
    QT += concurrent
    HEADERS += $${httpSourcesDir}/qhttpserverfutureresponse.h
    SOURCES += $${httpSourcesDir}/qhttpserverfutureresponse.cpp
}

include($${httpSourcesDir}/../3rdparty/http-parser.pri)
