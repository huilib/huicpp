##
## File Name: install.sh
## Create Author: Joseph Hui
## Create Date: Thu Apr 7 1101 2022
## Description:
## 		huicpp installation script.
##


#!/bin/bash

## variable for current direction.
NOWDIR=`pwd`

## module name.
PRONAME=huicpp
CTNUM=8

## common functions.
## create direction always.
function recreate_dir() {
	dd=$1
	if [ -d ${dd} ]
	then
		rm -fr ${dd};
	fi
	mkdir -p ${dd};
}

## output direction
DEPS_DIR=${NOWDIR}/deps

## deps source direction.
DEPSSRC_DIR=${NOWDIR}/depssrc

## if deps source direction is not exist, EXIT always.
if [ ! -d ${DEPSSRC_DIR} ]
then 
	echo "miss deps source dir[${DEPSSRC_DIR}]."
	exit -1
fi 

## remove output direction first.
rm -fr ${DEPS_DIR}

chmod +x *.sh

## compile all
source ${NOWDIR}/install_linux64.sh
#source ${NOWDIR}/install_arm1126.sh
#source ${NOWDIR}/install_arm3399.sh

## build test
pushd test 
	chmod +x build_test.sh
	source build_test.sh
popd

cd ${NOWDIR}
zip -r deps.zip deps
