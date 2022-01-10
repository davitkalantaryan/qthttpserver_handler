#!/bin/bash

#
# set following environment variables
# 1. QT_ROOT_DIR   example => $ export QT_ROOT_DIR=~/qt_compiled/5.15.2
#                               export QT_ROOT_DIR=~/Qt/5.15.2

# script to prepare developer host, to work with the code on this repo

#scriptFileFullPath=`readlink -f ${0}`
#scriptDirectory=`dirname ${scriptFileFullPath}`
#cd ${scriptDirectory}/../..
#repositoryRoot=`pwd`

# in mac os above short calculation will not work
# also realpath utilit is missing in mac

scriptDirectory=`dirname "${0}"`
scriptFileName=`basename "${0}"`
cd "${scriptDirectory}"
fileOrigin=`readlink "${scriptFileName}"` || :
while [ ! -z "${fileOrigin}" ]
do
	scriptDirectory=`dirname "${fileOrigin}"`
	scriptFileName=`basename "${fileOrigin}"`
	cd "${scriptDirectory}"
	fileOrigin=`readlink "${scriptFileName}"`  || :
done
cd ../..
repositoryRoot=`pwd`
echo repositoryRoot=$repositoryRoot

if [[ "$(uname)" == "Darwin" ]]; then
	# Do something under Mac OS X platform
	lsbCode=mac
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" ]]; then
	# Do something under GNU/Linux platform
	lsbCode=`lsb_release -sc`
#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]]; then
#	# Do something under 32 bits Windows NT platform
#elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]]; then
#	# Do something under 64 bits Windows NT platform
#else
fi


# https://intoli.com/blog/exit-on-errors-in-bash-scripts/
# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
#trap 'echo "\"${last_command}\" command finished with exit code $?."' EXIT

if [ -z "$QT_ROOT_DIR" ]; then
	echo "QT_ROOT_DIR environment variable is not set"
	exit 1
fi


if [[ "$2" == "Debug" ]] || [[ "$2" == "debug" ]]; then
	CONFIG_ADD="CONFIG-=release CONFIG+=debug"
	Configuration=Debug
else
	CONFIG_ADD="CONFIG+=release CONFIG-=debug"
	Configuration=Release
fi
echo $CONFIG_ADD

cd ${repositoryRoot}/prj/core/corelibs_qt
rm -f  Makefile.*
rm -f  .qmake.stash
rm -fr ${repositoryRoot}/sys/${lsbCode}/${Configuration}/.objects/QtSslServer
rm -fr ${repositoryRoot}/sys/${lsbCode}/${Configuration}/.objects/QtHttpServer
rm -f  ${repositoryRoot}/sys/${lsbCode}/${Configuration}/lib/libQtSslServer.so*
rm -f  ${repositoryRoot}/sys/${lsbCode}/${Configuration}/lib/libQtHttpServer.so*


${QT_ROOT_DIR}/$1/bin/qmake sslserver.pro ${CONFIG_ADD}
make -f Makefile.QtSslServer.${lsbCode}.${Configuration}
${QT_ROOT_DIR}/$1/bin/qmake httpserver.pro ${CONFIG_ADD}
make -f Makefile.QtHttpServer.${lsbCode}.${Configuration}

