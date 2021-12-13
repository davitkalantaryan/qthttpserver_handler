
#include <QCoreApplication>
#include "multibackend_http_server.hpp"
#include <QDebug>


int main(int a_argc, char* a_argv[])
{
	QCoreApplication aApp(a_argc,a_argv);
	const char* crtFileName = "crtFileName";
	const char* keyFileName = "keyFileName";
	if(a_argc>1){crtFileName=a_argv[1];}
	if(a_argc>2){keyFileName=a_argv[2];}
	qthttpserverhandler::MultibackendHttpServer aServer(a_argv[0],crtFileName,keyFileName);
	int nReturn = QCoreApplication::exec();	
    return nReturn;
}
