
#include <qthttpserverhandler/qthttpserverhandler_internal_header.h>
#include <qthttpserverhandler/multibackend_http_server.h>



QTHTTPSERVERHANDLER_BEGIN_C

QTHTTPSERVERHANDLER_DLL_PUBLIC QHttpServerResponse* QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME(const QHttpServerRequest& a_request, QTcpSocket* a_socket)
{
	static_cast<void>(a_socket);
	const QUrl aUrl = a_request.url();
	return new QHttpServerResponse("Responce from plugin url:" + aUrl.toString());
}

QTHTTPSERVERHANDLER_END_C
