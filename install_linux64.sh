###
 # @Author: Joseph Hui
 # @Date: 2022-03-11 17:56:56
 # @Description: 
###

#!/bin/bash

PLATFROM=linux
PLATCODE=64
PLATSYSCODE=${PLATFROM}${PLATCODE}

GCC_HEADER=

OUTPUT_DIR=${DEPS_DIR}/${PLATSYSCODE}
TEMP_DEPS=${DEPSSRC_DIR}/${PLATSYSCODE}

source ${NOWDIR}/deps_install_tail.sh

## compile huicpp 
cd ${NOWDIR}
sed -e "s%__1__WITH_DESP__1__%HUI_ITEMS += crypto%g" makefile.src > makefile
sed -e "s%__1__DEPS_HOME_PATH__1__%${NOWDIR}/deps/${PLATSYSCODE}%g" mkvars.mk.src > mkvars.mk
sed -i "s%__1__INSTALL_OUTPUT__1__%${PREFIX_DIR}%g" mkvars.mk
sed -e "s%__1__PLATSYSCODE__1__%${PLATSYSCODE}%g"  inc.mk.src > inc.mk
sed -i "s%_2_OS_COMPILE_VARS_2_%-DX86JSPP%g"  inc.mk
make clean
make
#make install

rm -f inc.mk
rm -f mkvars.mk
#rm -f makefile


### copy huicpp to deps
output_dep=${NOWDIR}/deps/${PLATSYSCODE}
pushd ${output_dep}
	recreate_dir huicpp
	pushd huicpp
		#### src
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
			### crypto
			mkdir -p crypto
			pushd crypto
				cp ${NOWDIR}/crypto/*.h ./
			popd 
			### db
			# mkdir -p db
			# pushd db
			# 	cp ${NOWDIR}/db/*.h ./
			# popd 
		popd		
		pushd lib
			cp -r ${NOWDIR}/lib/${PLATSYSCODE}/* ./
		popd
	popd
popd 

