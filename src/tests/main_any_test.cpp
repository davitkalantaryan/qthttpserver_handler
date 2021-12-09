
#include <QCoreApplication>
#include <QtHttpServer>
#include <QDebug>

int main(int a_argc, char* a_argv[])
{
	QCoreApplication aApp(a_argc,a_argv);
	QHttpServer httpServer;
	
	qDebug()<<QSslConfiguration::defaultConfiguration().isNull();
	
	QSslConfiguration sslConfiguration;
	//QFile certFile(QStringLiteral("./localhost.cert"));
	//QFile keyFile(QStringLiteral("./localhost.key"));
	QFile certFile(QStringLiteral("D:\\davit\\dev\\qthttpserver_handler\\.tests\\certificate.crt"));
	QFile keyFile(QStringLiteral("D:\\davit\\dev\\qthttpserver_handler\\.tests\\privatekey.key"));
	certFile.open(QIODevice::ReadOnly);
	keyFile.open(QIODevice::ReadOnly);
	QSslCertificate certificate(&certFile, QSsl::Pem);
	QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
	certFile.close();
	keyFile.close();
	sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
	sslConfiguration.setLocalCertificate(certificate);
	sslConfiguration.setPrivateKey(sslKey);
	sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
	httpServer.sslSetup(sslConfiguration);
	
	httpServer.route("/", []() {
        return "Hello world";
    });
	
	//httpServer.route("/files/<arg>", [] (const QUrl &url) {
	//	qDebug()<<"New querry: "<<url;
    //    return QHttpServerResponse::fromFile(url.toString());
    //});
	
	QObject::connect(&httpServer,&QHttpServer::missingHandler,&httpServer,[](const QHttpServerRequest &request, QTcpSocket *socket){
		qDebug()<<"!!! request:"<<request;
		qDebug()<<"+++ socket:"<< socket;
	});
	
	const auto port = httpServer.listen(QHostAddress::Any,443);
    if (!port) {
        qDebug() << QCoreApplication::translate(
                "QHttpServerExample", "Server failed to listen on a port.");
        return 0;
    }

    qDebug() << QCoreApplication::translate(
            "QHttpServerExample", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

    return aApp.exec();
}
