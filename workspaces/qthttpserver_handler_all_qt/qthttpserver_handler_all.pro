#
# file:			qthttpserver_handler_all.pro
# path:			workspaces/qthttpserver_handler_all_qt/qthttpserver_handler_all.pro
# created on:		2021 Dec 08
# creatd by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
# purpose:		Qt Project file to handle monitor project related files
#


message("!!!!!!!!!!! qthttpserver_handler_all.pro")

TEMPLATE = subdirs
CONFIG += ordered
OUT_PWD = $${PWD}

repositoryRoot = $${PWD}/../..

SUBDIRS		+=	"$${repositoryRoot}/prj/core/core_qt/sslserver.pro"
SUBDIRS		+=	"$${repositoryRoot}/prj/core/core_qt/httpserver.pro"
SUBDIRS		+=	"$${repositoryRoot}/prj/tests/servtest01_qt/servtest01.pro"

$${repositoryRoot}/prj/core/core_qt/httpserver.pro.depends = $${repositoryRoot}/prj/core/core_qt/sslserver.pro
$${repositoryRoot}/prj/tests/servtest01_qt/servtest01.pro.depends = $${repositoryRoot}/prj/core/core_qt/httpserver.pro


UNIX_SCRIPTS	=  $$files($${repositoryRoot}/scripts/*.sh,true)
UNIX_SCRIPTS	+= $${repositoryRoot}/scripts/prepare_files
WINDOWS_SCRIPTS	=  $$files($${repositoryRoot}/scripts/*.bat,true)
# MDS_IN_DOCS	=  $$files($${repositoryRoot}/docs/*.md,true)
# TXT_IN_DOCS	=  $$files($${repositoryRoot}/docs/*.txt,true)

OTHER_FILES	=
OTHER_FILES += $${UNIX_SCRIPTS}
OTHER_FILES += $${WINDOWS_SCRIPTS}
OTHER_FILES += $${MDS_IN_DOCS}
OTHER_FILES += $${TXT_IN_DOCS}

OTHER_FILES	+=	\
	"$${repositoryRoot}/.gitattributes"								\
	"$${repositoryRoot}/.gitignore"									\
	"$${repositoryRoot}/.gitmodules"								\
	"$${repositoryRoot}/LICENSE"									\
	"$${repositoryRoot}/README.md"									\
	                                                                                                \
	"$${repositoryRoot}/.github/workflows/test_pull_to_master.yml"					\
	                                                                                                \
	"$${repositoryRoot}/prj/common/common_mkfl/monitor.unix.common.Makefile"