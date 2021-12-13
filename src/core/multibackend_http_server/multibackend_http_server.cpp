//
// file:			multibackend_http_server.cpp
// path:			${repositoryRoot}/src/core/multibackend_http_server/multibackend_http_server.cpp
// created on:		2021 Dec 13
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#include "multibackend_http_server.hpp"
#include <qthttpserverhandler/multibackend_http_server.h>
#include <functional>
#include <QDir>
#include <QFile>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QtHttpServer/QtHttpServer>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace qthttpserverhandler{

//#define HTTP_SERV_PORT	80
//#define HTTPS_SERV_PORT	443
#define HTTP_SERV_PORT	9031
#define HTTPS_SERV_PORT	9032

#define QTHTTPSERVERHANDLER_ANY_FILE	"/____files/"
static constexpr int s_cnAnyFileLenPlus1 = 11;

#ifdef _WIN32
#define QthttpserverhandlerLoadLib				LoadLibraryA
#define QthttpserverhandlerFreeLib				FreeLibrary
#define QthttpserverhandlerGetLibSym			GetProcAddress
#define QthttpserverhandlerLibExt				"dll"
typedef  :: HMODULE		HMODULE_t;
#else
#define QthttpserverhandlerLoadLib(_libName)	dlopen(_libName,RTLD_LAZY)
#define QthttpserverhandlerFreeLib				dlclose
#define QthttpserverhandlerGetLibSym			dlsym
typedef  void*			HMODULE_t;
#define QthttpserverhandlerLibExt				"so"
#endif


static QHttpServerResponse HandleRequestDefault(bool* a_pbRet, const QHttpServerRequest&, QTcpSocket*){*a_pbRet= false;return QHttpServerResponse("Bad request");}

class DataCleaner{
public:
	typedef ::std::function<void()>	TypeCleaner;
	DataCleaner(const TypeCleaner& a_cleaner):m_cleaner(a_cleaner){}
	~DataCleaner(){m_cleaner();}
private:
	const TypeCleaner	m_cleaner;
};

struct SSubdomainHandlerFnc{
	SSubdomainHandlerFnc(TypeHandleRequest a_fpFnc):fpFnc(a_fpFnc?a_fpFnc:HandleRequestDefault),prev(nullptr),next(nullptr){}
	TypeHandleRequest		fpFnc;
	SSubdomainHandlerFnc	*prev,*next;
};


struct SSubdomainHandler{
	HMODULE_t				libModule;
	SSubdomainHandlerFnc*	fpFncUnS;
	SSubdomainHandlerFnc*	fpFncSafe;
	SSubdomainHandler		*prev,*next;
	SSubdomainHandler(HMODULE_t a_libModule, TypeHandleRequest a_fpFncUnSf, TypeHandleRequest a_fpFncSf):libModule(a_libModule),prev(nullptr),next(nullptr){
		this->fpFncUnS = new SSubdomainHandlerFnc(a_fpFncUnSf);
		this->fpFncSafe = new SSubdomainHandlerFnc(a_fpFncSf);
	}
	~SSubdomainHandler(){
		delete this->fpFncSafe;
		delete this->fpFncUnS;
	}
};


class HttpServerBase : public QAbstractHttpServer{
public:
	HttpServerBase(MultibackendHttpServer_p* a_pParent);
	void sendResponse(QHttpServerResponse &&response,const QHttpServerRequest &request,QTcpSocket *socket);
protected:
	MultibackendHttpServer_p*const	m_pParent;
};


class HttpServer final : public HttpServerBase{
public:
	using HttpServerBase::HttpServerBase;
private:
	bool handleRequest(const QHttpServerRequest &request, QTcpSocket *socket) override;
};

class HttpsServer final : public HttpServerBase{
public:
	using HttpServerBase::HttpServerBase;
private:
	bool handleRequest(const QHttpServerRequest &request, QTcpSocket *socket) override;
};


class QTHTTPSERVERHANDLER_DLL_PRIVATE MultibackendHttpServer_p final : public QThread{
public:
	MultibackendHttpServer_p(const char* a_argv0, const QString& a_crtFileName, const QString& a_keyFileName);
	inline void HandleClientRequestNotSafe(HttpServer* a_pServer, const QHttpServerRequest& a_request, QTcpSocket* a_socket);
	inline bool HandleClientRequest(HttpServerBase* a_pServer,const QHttpServerRequest& a_request, QTcpSocket* a_socket, SSubdomainHandlerFnc* a_pFirst);
	inline QHttpServerResponse HandleAnyResponce(const QString& a_url, bool a_bAnyFileAccepted);
	inline QHttpServerResponse HandleAnyResponceFromUrl(const QUrl& a_url);
private:
	void run() override;
	inline void DefaultrequestHandler(HttpServer* a_pServer, const QHttpServerRequest& a_request, QTcpSocket* a_socket);
	inline void ScanAndFindAllModules();
	inline void TryToLoaPluginAndGetSymbols(const QString& pluginName);
public:
	const char*const		m_argv0;
	const QString			m_crtFileName;
	const QString			m_keyFileName;
	SSubdomainHandler*		m_pFirstLib;     // leavs in worker thread
	SSubdomainHandlerFnc*	m_pFirstFncUnSf; // leavs in worker thread
	SSubdomainHandlerFnc*	m_pFirstFncSafe; // leavs in worker thread
};


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


MultibackendHttpServer::MultibackendHttpServer(const char* a_argv0, const QString& a_crtFileName, const QString& a_keyFileName)
	:
	  m_data_ptr(new MultibackendHttpServer_p(a_argv0,a_crtFileName,a_keyFileName))
{
	m_data_ptr->start();
}


MultibackendHttpServer::~MultibackendHttpServer()
{
	m_data_ptr->quit();
	m_data_ptr->wait();
	delete m_data_ptr;
}



/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

MultibackendHttpServer_p::MultibackendHttpServer_p(const char* a_argv0,const QString& a_crtFileName, const QString& a_keyFileName)
	:
	  m_argv0(a_argv0),
	  m_crtFileName(a_crtFileName),
	  m_keyFileName(a_keyFileName)
{
	m_pFirstLib=nullptr;
	m_pFirstFncUnSf=nullptr;
	m_pFirstFncSafe=nullptr;
}


void MultibackendHttpServer_p::run()
{
	HttpServer httpServer(this);
	HttpsServer httpsServer(this);
	
	QDir appDir = QFileInfo(m_argv0).dir().path();
	QString crtFilePath = QFileInfo(appDir,m_crtFileName).filePath();
	QString keyFilePath = QFileInfo(appDir,m_keyFileName).filePath();
	QFile crtFile(crtFilePath);
	QFile keyFile(keyFilePath);
	QTimer scanTimer;
	
	DataCleaner aCleaner([this,&scanTimer](){
		SSubdomainHandler *pTmp, *pElem=m_pFirstLib;
		scanTimer.stop();
		while(pElem){
			pTmp = pElem->next;
			QthttpserverhandlerFreeLib(pElem->libModule);
			delete (pElem);
			pElem = pTmp;
		}
	});
	
	crtFile.open(QIODevice::ReadOnly);
	keyFile.open(QIODevice::ReadOnly);
		
	if(crtFile.isOpen() && keyFile.isOpen()){
		qDebug()<<QSslConfiguration::defaultConfiguration().isNull();
		QSslConfiguration sslConfiguration;
		QSslCertificate certificate(&crtFile, QSsl::Pem);
		QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
		crtFile.close();
		keyFile.close();
		sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
		sslConfiguration.setLocalCertificate(certificate);
		sslConfiguration.setPrivateKey(sslKey);
		sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
		httpsServer.sslSetup(sslConfiguration);
		const auto portS = httpsServer.listen(QHostAddress::Any,HTTPS_SERV_PORT);
		if (portS) {
			qDebug() << QCoreApplication::translate("HttpServer", "Running on https://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(portS) ;
		}
		else{
			qDebug() << QCoreApplication::translate("HttpServer", "Server failed to listen on a port.");
		}
	}
	else{
		qCritical()<<"Unable to get certificates";
	}
	
	const auto port = httpServer.listen(QHostAddress::Any,HTTP_SERV_PORT);
	if (port) {
		qDebug() << QCoreApplication::translate("HttpServer", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port) ;
	}
	else{
		qDebug() << QCoreApplication::translate("HttpServer", "Server failed to listen on a port.");
	}
	
	ScanAndFindAllModules();
	scanTimer.start(10000);
	
	QObject::connect(&scanTimer,&QTimer::timeout,&scanTimer,[this](){
        ScanAndFindAllModules();
    });
		
	//QObject::connect(&httpServer2,&HttpServer::missingHandler,&httpServer2,[this](const QHttpServerRequest& a_request, QTcpSocket* a_socket){
	//	HttpServer* pServer = static_cast<HttpServer*>(sender());
	//	HandleClientRequestNotSafe(pServer,a_request,a_socket);
	//});
	//
	//QObject::connect(&httpsServer2,&HttpServer::missingHandler,&httpsServer2,[this](const QHttpServerRequest& a_request, QTcpSocket* a_socket){
	//	HttpServer* pServer = static_cast<HttpServer*>(sender());
	//	HandleClientRequest(pServer,a_request,a_socket,m_pFirstSafe);
	//});
	
	QThread::run();
	
}


inline bool MultibackendHttpServer_p::HandleClientRequest(HttpServerBase* a_pServer,const QHttpServerRequest& a_request, QTcpSocket* a_socket, SSubdomainHandlerFnc* a_pFirst)
{
	::std::shared_ptr<QHttpServerResponse> pResponse;
	SSubdomainHandlerFnc* pHandler = a_pFirst;
	bool bRespond = false;
	while(pHandler){
		QHttpServerResponse aResponse = (*pHandler->fpFnc)(&bRespond,a_request,a_socket);
		if(bRespond){
			a_pServer->sendResponse(std::move(aResponse), a_request, a_socket);
			return true;
		}
		pHandler = pHandler->next;
	}
	
	return false;
}


inline QHttpServerResponse MultibackendHttpServer_p::HandleAnyResponce(const QString& a_url, bool a_bAnyFileAccepted)
{
	QString filePath(a_url);
	if(a_bAnyFileAccepted){
		if((filePath.size()>1) && (filePath.at(0)=='~')){
			filePath.replace(0,1,QDir::homePath());
			qDebug()<<"new path: "<<filePath;
		}
	}
	QFileInfo aFileInfo(filePath);
	if(aFileInfo.isFile()){
		return QHttpServerResponse::fromFile(filePath);
	}
	
	return QHttpServerResponse(QString("Resource \"") + aFileInfo.filePath() + "\" is not available");
}


inline QHttpServerResponse MultibackendHttpServer_p::HandleAnyResponceFromUrl(const QUrl& a_url)
{
	QDir appDir = QFileInfo(m_argv0).dir().path();
	QDir oneUpDir = QFileInfo(appDir.path()).dir().path();
	QString requestFilePath = QFileInfo(oneUpDir,a_url.toString()).filePath();
	return HandleAnyResponce(requestFilePath,false);
}


inline void MultibackendHttpServer_p::DefaultrequestHandler(HttpServer* a_pServer, const QHttpServerRequest& a_request, QTcpSocket* a_socket)
{
	const QUrl aUrl = a_request.url();
	const QString strPath = aUrl.path();
	
	qDebug()<<"aUrl.path(): "<<aUrl.path();
	
	if(strPath.isEmpty()||((strPath.size()==1)&&(strPath[0]=='/'))){
		a_pServer->sendResponse(HandleAnyResponceFromUrl(QUrl("index.html")), a_request, a_socket);
		return;
	}
	
	if(strPath.startsWith("/____files/")){
		QString afterDelim = strPath.mid(s_cnAnyFileLenPlus1);
		a_pServer->sendResponse(HandleAnyResponceFromUrl(QUrl(afterDelim)), a_request, a_socket);
		return;
	}
	
	a_pServer->sendResponse(HandleAnyResponce(strPath,true), a_request, a_socket);
}


inline void MultibackendHttpServer_p::TryToLoaPluginAndGetSymbols(const QString& a_pluginName)
{
	HMODULE_t libHandle = QthttpserverhandlerLoadLib(a_pluginName.toStdString().c_str());
	if(libHandle){
		void *pUnSafeFnc, *pSafeFnc;
		pUnSafeFnc = QthttpserverhandlerGetLibSym(libHandle,QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_STR);
		pSafeFnc = QthttpserverhandlerGetLibSym(libHandle,QTHTTPSERVERHANDLER_MULTIBACKEND_FNC_NAME_S_STR);
		if(pUnSafeFnc||pSafeFnc){
			SSubdomainHandler* pLibHandle = new SSubdomainHandler(libHandle,
																  reinterpret_cast<TypeHandleRequest>(pUnSafeFnc),
																  reinterpret_cast<TypeHandleRequest>(pSafeFnc));
			{
				if(m_pFirstLib){
					m_pFirstLib->prev = pLibHandle;
				}
				pLibHandle->next = m_pFirstLib;
				m_pFirstLib = pLibHandle;
			}
			{
				SSubdomainHandlerFnc* pFirstFnc = pLibHandle->fpFncUnS;
				if(m_pFirstFncUnSf){
					m_pFirstFncUnSf->prev = pFirstFnc;
				}
				pFirstFnc->next = m_pFirstFncUnSf;
				m_pFirstFncUnSf = pFirstFnc;
			}
			{
				SSubdomainHandlerFnc* pFirstFnc = pLibHandle->fpFncSafe;
				if(m_pFirstFncSafe){
					m_pFirstFncSafe->prev = pFirstFnc;
				}
				pFirstFnc->next = m_pFirstFncSafe;
				m_pFirstFncSafe = pFirstFnc;
			}
		} // if(pSaveFnc||pUnSafeFnc){
	}  // if(libHandle){
}


inline void MultibackendHttpServer_p::ScanAndFindAllModules()
{
	QDir appDir = QFileInfo(m_argv0).dir().path();
	QDir pluginsDirPath = QFileInfo(appDir,"plugins").filePath();
	
	if(!pluginsDirPath.exists()){
		qCritical()<<"Plugins directory does not exist";
		return;
	}
	
	pluginsDirPath.setFilter(QDir::Files);
	QFileInfoList list = pluginsDirPath.entryInfoList();
	const int cnCount = list.size();
	
	for(int i(0);i<cnCount;++i){
		const QFileInfo aFileIfo = list.at(i);
		const QString aFilePath = aFileIfo.filePath();
		if(aFilePath.contains("." QthttpserverhandlerLibExt)){
			TryToLoaPluginAndGetSymbols(aFilePath);
		}
	}
}


inline void MultibackendHttpServer_p::HandleClientRequestNotSafe(HttpServer* a_pServer,const QHttpServerRequest& a_request, QTcpSocket* a_socket)
{
	if(HandleClientRequest(a_pServer,a_request,a_socket,m_pFirstFncUnSf)){
		return;
	}
	
	DefaultrequestHandler(a_pServer,a_request,a_socket);
}


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

HttpServerBase::HttpServerBase(MultibackendHttpServer_p* a_pParent)
	:
	  m_pParent(a_pParent)
{
}

void HttpServerBase::sendResponse(QHttpServerResponse &&response,
                               const QHttpServerRequest &request,
                               QTcpSocket *socket)
{
    //Q_D(HttpServer);
    //for (auto afterRequestHandler : d->afterRequestHandlers)
    //    response = std::move(afterRequestHandler(std::move(response), request));
    response.write(makeResponder(request, socket));
}


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

bool HttpServer::handleRequest(const QHttpServerRequest& a_request, QTcpSocket* a_socket)
{
	m_pParent->HandleClientRequestNotSafe(this,a_request,a_socket);
	return true;
}


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

bool HttpsServer::handleRequest(const QHttpServerRequest& a_request, QTcpSocket* a_socket)
{
	return m_pParent->HandleClientRequest(this,a_request,a_socket,m_pParent->m_pFirstFncSafe);
}


}  // namespace qthttpserverhandler{
