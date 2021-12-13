//
// file:			multibackend_http_server.hpp
// path:			${repositoryRoot}/src/core/multibackend_http_server/multibackend_http_server.hpp
// created on:		2021 Dec 13
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#pragma once

#include <qthttpserverhandler/qthttpserverhandler_internal_header.h>
#include <QString>

namespace qthttpserverhandler{


class QTHTTPSERVERHANDLER_DLL_PRIVATE MultibackendHttpServer_p;

class QTHTTPSERVERHANDLER_EXPORT MultibackendHttpServer final
{
public:
	MultibackendHttpServer(const char* a_argv0, const QString& a_crtFileName, const QString& a_keyFileName);
	~MultibackendHttpServer();
		
private:
	MultibackendHttpServer_p*const m_data_ptr;
};


}  // namespace qthttpserverhandler{
