//
// file:			multibackend_http_server.h
// path:			${repositoryRoot}/include/qthttpserverhandler/multibackend_http_server.h
// created on:		2021 Dec 13
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef INCLUDE_QTHTTPSERVERHANDLER_MULTIBACKEND_HTTP_SERVER_H
#define INCLUDE_QTHTTPSERVERHANDLER_MULTIBACKEND_HTTP_SERVER_H

#include <qthttpserverhandler/qthttpserverhandler_internal_header.h>
#include <QtHttpServer/qhttpserverresponse.h>
#include <QtHttpServer/qhttpserverrequest.h>
#include <memory>
#include <stdbool.h>

// QTHTTPSERVERHANDLER_STRINGIFY

#define QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME			multibackend_handler_function
#define QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_S			multibackend_handler_function_s

#define QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_STR		QTHTTPSERVERHANDLER_STRINGIFY(QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME)
#define QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_S_STR		QTHTTPSERVERHANDLER_STRINGIFY(QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_S)


QTHTTPSERVERHANDLER_BEGIN_C

typedef QHttpServerResponse (*TypeHandleRequest)(bool* a_pResponce, const QHttpServerRequest& request, QTcpSocket* socket);


QTHTTPSERVERHANDLER_END_C

#endif  // #ifndef INCLUDE_QTHTTPSERVERHANDLER_MULTIBACKEND_HTTP_SERVER_H
