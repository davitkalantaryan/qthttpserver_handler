
#include <QCoreApplication>
#include <QtHttpServer>
#include <QThread>
#include <QFileInfo>
#include <QDebug>

#define HTTP_SERV_PORT	80
#define HTTPS_SERV_PORT	443


class HttpThreadBase : public QThread
{
protected:
	HttpThreadBase(const char* a_arg0);
	void RunReimplemented(QHttpServer* a_pHttpServer);
protected:
private:
	const char*const m_argv0;
};


class HttpThread : public HttpThreadBase
{
public:
	using HttpThreadBase::HttpThreadBase;
private:
	void run() override;
};

class HttpsThread : public HttpThreadBase
{
public:
	using HttpThreadBase::HttpThreadBase;
private:
	void run() override;
};


int main(int a_argc, char* a_argv[])
{
	QCoreApplication aApp(a_argc,a_argv);
	HttpThread httpThread;
	HttpsThread httpsThread(a_argv[0]);
		
	httpThread.start();
	httpsThread.start();
	
	int nReturn = QCoreApplication::exec();
	
	httpsThread.quit();
	httpsThread.wait();
	httpThread.quit();
	httpThread.wait();
	
    return nReturn;
}


/*//////*/

void HttpThread::run()
{
	QHttpServer httpServer;
	HttpThreadBase::RunReimplemented(&httpServer);
}


/*//////*/

void HttpsThread::run()
{
	QDir appDir = QFileInfo(m_argv0).dir().path();
	QString certFilePath = QFileInfo(appDir,"_.smartdav.de_.cert").filePath();
	QString keyFilePath = QFileInfo(appDir,"_.smartdav.de_private_key.key").filePath();
	QFile certFile(certFilePath);
	QFile keyFile(keyFilePath);
	certFile.open(QIODevice::ReadOnly);
	keyFile.open(QIODevice::ReadOnly);
	
	if(certFile.isOpen() && keyFile.isOpen()){
		qDebug()<<QSslConfiguration::defaultConfiguration().isNull();
		QHttpServer httpServer;
		QSslConfiguration sslConfiguration;
		QSslCertificate certificate(&certFile, QSsl::Pem);
		QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
		certFile.close();
		keyFile.close();
		sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
		sslConfiguration.setLocalCertificate(certificate);
		sslConfiguration.setPrivateKey(sslKey);
		sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
		httpServer.sslSetup(sslConfiguration);		
		HttpThreadBase::RunReimplemented(&httpServer);
	}
	else{
		QThread::run();
	}
}


/*//////*/

HttpThreadBase::HttpsThread(const char* a_arg0)
	:
	  m_argv0(a_arg0)
{
}

void HttpThreadBase::RunReimplemented(QHttpServer* a_pHttpServer)
{
	QHttpServer& httpServer = *a_pHttpServer;
	HttpsThread* pIsSafe = dynamic_cast<HttpsThread*>(this);
	
	httpServer.route("/____files/<arg>", [] (const QUrl &url) {
		qDebug()<<"File querry: "<<url;
		QString filePath(url.toString());
		if((filePath.size()>1) && (filePath.at(0)=='~')){
			filePath.replace(0,1,QDir::homePath());
			qDebug()<<"new path: "<<filePath;
		}
		QFileInfo aFileInfo(filePath);
		if(aFileInfo.isFile()){
			return QHttpServerResponse::fromFile(filePath);
		}
		
		return QHttpServerResponse(QString("File \"") + aFileInfo.filePath() + "\" is not available");
    });
	
	httpServer.route("/index.html", []() {
        //return QHttpServerResponse::fromFile(QStringLiteral("index.html"));
		QDir appDir = QFileInfo(m_argv0).dir().path();
		QDir indexFileDir = QFileInfo(appDir.path()).dir().path();
		QString indexFilePath = QFileInfo(indexFileDir,"index.html").filePath();
		return QHttpServerResponse::fromFile(indexFilePath);
    });
	
	httpServer.route("/", []() {
        //return QHttpServerResponse::fromFile(QStringLiteral("index.html"));
		QDir appDir = QFileInfo(m_argv0).dir().path();
		QDir indexFileDir = QFileInfo(appDir.path()).dir().path();
		QString indexFilePath = QFileInfo(indexFileDir,"index.html").filePath();
		return QHttpServerResponse::fromFile(indexFilePath);
    });
	
	//QObject::connect(&httpServer,&QHttpServer::missingHandler,&httpServer,[](const QHttpServerRequest &request, QTcpSocket *socket){
	//	qDebug()<<"!!! request:"<<request;
	//	qDebug()<<"+++ socket:"<< socket;
	//});
	
	//const auto port = httpServer.listen(QHostAddress::Any,443);
    //const auto port = httpServer.listen(QHostAddress::Any,80);
    //const auto port = httpServer.listen(QHostAddress::Any);
	const auto port = httpServer.listen(QHostAddress::Any,pIsSafe?HTTPS_SERV_PORT:HTTP_SERV_PORT);
    if (!port) {
        qDebug() << QCoreApplication::translate(
                "QHttpServerExample", "Server failed to listen on a port.");
        return;
    }

    qDebug() << (  pIsSafe? 
					QCoreApplication::translate("QHttpServerExample", "Running on https://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port)  : 
					QCoreApplication::translate("QHttpServerExample", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port)  ) ;
	
	QThread::run();
}
