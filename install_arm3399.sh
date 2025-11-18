###
 # @Author: Joseph Hui
 # @Date: 2022-03-11 17:56:56
 # @Description: 
###

#!/bin/bash

PLATFROM=arm
PLATCODE=3399
PLATSYSCODE=${PLATFROM}${PLATCODE}

GG_HOME=/root/.bins/aarch64-linux-gnu/bin/
GG_HEADER=${GG_HOME}aarch64-linux-gnu-

OUTPUT_DIR=${DEPS_DIR}/${PLATSYSCODE}
TEMP_DEPS=${DEPSSRC_DIR}/${PLATSYSCODE}

source ${NOWDIR}/deps_install_tail.sh

## compile huicpp 
cd ${NOWDIR}
sed -e "s%__1__DEPS_HOME_PATH__1__%${NOWDIR}/deps/${PLATSYSCODE}%g" mkvars.mk.src > mkvars.mk
sed -e "s%__1__WITH_DESP__1__%#ARM_WITH_OUT_DB%g" makefile.src > makefile
sed -i "s%__1__INSTALL_OUTPUT__1__%${PREFIX_DIR}%g" mkvars.mk
sed -e "s%__1__PLATSYSCODE__1__%${PLATSYSCODE}%g"  inc.mk.src > inc.mk
sed -i "s%_2_OS_COMPILE_VARS_2_%-DARMJSPP%g"  inc.mk
sed -i "s%GG_HEADER = %GG_HEADER = ${GG_HEADER}%g" inc.mk
make clean
make
#make install

rm -f inc.mk
rm -f mkvars.mk
rm -f makefile



### copy huicpp to deps
output_dep=${NOWDIR}/deps/${PLATSYSCODE}
pushd ${output_dep}
	recreate_dir huicpp
	pushd huicpp
		mkdir -p lib include
		pushd include 
			mkdir adaptation detail detail/log inl internal process utils image baseinside
			cp ${NOWDIR}/src/*.h ./
			cp ${NOWDIR}/src/adaptation/*.h ./adaptation/
			cp ${NOWDIR}/src/detail/*.h ./detail/
			cp ${NOWDIR}/src/detail/log/*.h ./detail/log/
			cp ${NOWDIR}/src/inl/*.h ./inl/
			cp ${NOWDIR}/src/internal/*.h ./internal/
			cp ${NOWDIR}/src/process/*.h ./process/
			cp ${NOWDIR}/src/utils/*.h ./utils/
			cp ${NOWDIR}/src/image/*.h ./image/
			cp ${NOWDIR}/src/baseinside/*.h ./baseinside/
		popd
		pushd lib
			cp -r ${NOWDIR}/lib/${PLATSYSCODE}/* ./
		popd
	popd
popd 
