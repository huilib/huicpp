#!/bin/bash


recreate_dir ${TEMP_DEPS}
cd ${DEPSSRC_DIR}

## compile openSSL
tar xfz openssl.tar.gz -C ${TEMP_DEPS}
OPENSSL_OUTPUT_DIR=${OUTPUT_DIR}/openssl
recreate_dir ${OPENSSL_OUTPUT_DIR}
pushd ${TEMP_DEPS}/openssl
    if [ "${PLATSYSCODE}" == "linux64" ]
    then 
        ./config --prefix=${OPENSSL_OUTPUT_DIR} -fPIC --shared;
    else
        setarch i386 ./config no-asm --prefix=${OPENSSL_OUTPUT_DIR} -fPIC --shared;
        sed -i "s%CROSS_COMPILE=%CROSS_COMPILE=${GG_HEADER}%g" Makefile
        sed -i "s%-m32%%g" Makefile
        sed -i "s/[ ]*$//g" Makefile
    fi
    make -j${CTNUM};
    make install;
popd 
pushd ${OPENSSL_OUTPUT_DIR}
	rm -fr bin share ssl
popd 

## compile curl.
if [ "${PLATSYSCODE}" == "linux64" ]
then
    ## compile libcurl
    tar xfz curl.tar.gz -C ${TEMP_DEPS}
    CURL_OUTPUT_DIR=${OUTPUT_DIR}/curl
    recreate_dir ${CURL_OUTPUT_DIR}
    pushd ${TEMP_DEPS}/curl
        PKG_CONFIG_PATH=${OPENSSL_OUTPUT_DIR}/lib/pkgconfig ./configure --prefix=${CURL_OUTPUT_DIR}
        make -j${CTNUM}
        make install
    popd 
    pushd ${CURL_OUTPUT_DIR}
        rm -fr share
    popd 
fi

## compile json
JSON_OUTPUT_DIR=${OUTPUT_DIR}/json
recreate_dir ${JSON_OUTPUT_DIR}
tar xfz nlohmann.tar.gz -C ${TEMP_DEPS}
cp -r ${TEMP_DEPS}/nlohmann  ${JSON_OUTPUT_DIR}/

## compile db librarys.
# if [ "${PLATSYSCODE}" == "linux64" ]
# then
#     ## compile libmysqlclient
#     tar xfz mysql.tar.gz -C ${TEMP_DEPS}
#     MYSQL_OUTPUT_DIR=${OUTPUT_DIR}/mysql
#     recreate_dir ${MYSQL_OUTPUT_DIR}
#     pushd ${TEMP_DEPS}/mysql
#         mkdir build
#         pushd build
#             cmake .. -DCMAKE_INSTALL_PREFIX=${MYSQL_OUTPUT_DIR}
#             make -j${CTNUM}
#             make install
#         popd
#     popd 
#     pushd ${MYSQL_OUTPUT_DIR}
#         rm -fr bin COPYING COPYING-debug docs README  README-debug
#     popd

#     ## compile mysql
#     SQLITE3_OUTPUT_DIR=${OUTPUT_DIR}/sqlite3
#     recreate_dir ${SQLITE3_OUTPUT_DIR}
#     tar xfz sqlite3.tar.gz -C ${TEMP_DEPS}
#     cp -r ${TEMP_DEPS}/sqlite3/*  ${SQLITE3_OUTPUT_DIR}/
# fi 

rm -fr ${TEMP_DEPS}

