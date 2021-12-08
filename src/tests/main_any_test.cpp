
#include <QCoreApplication>
#include <QtHttpServer>
#include <QDebug>

int main(int a_argc, char* a_argv[])
{
	QCoreApplication aApp(a_argc,a_argv);
	QHttpServer httpServer;
	
	httpServer.route("/", []() {
        return "Hello world";
    });
	
	httpServer.route("/files/<arg>", [] (const QUrl &url) {
		qDebug()<<"New querry: "<<url;
        return QHttpServerResponse::fromFile(url.toString());
    });
	
	const auto port = httpServer.listen(QHostAddress::Any);
    if (!port) {
        qDebug() << QCoreApplication::translate(
                "QHttpServerExample", "Server failed to listen on a port.");
        return 0;
    }

    qDebug() << QCoreApplication::translate(
            "QHttpServerExample", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

    return aApp.exec();
}
