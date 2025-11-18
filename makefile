##
## File Name: makefile
## Create Author: Joseph Hui
## Create Date: Thu Apr 7 1037 2022
## Description:
## 		huicpp's main makefile.
##

include mkvars.mk

.PHONY: all clean install deps test

HUI_ITEMS = 
HUI_ITEMS += crypto
#HUI_ITEMS = crypto http nosql
#HUI_ITEMS = http
DIRS = src ${HUI_ITEMS}

all:
	@for dir in ${DIRS}; do make -C $$dir -j${CTNUM}; echo ; done


clean:
	@for dir in ${DIRS}; do make -C $$dir clean; echo ; done


install:
	mkdir -p ${HUICPP_OUTPUT_LIB};
	mkdir -p ${HUICPP_OUTPUT_INCLUDE};
	cp -r lib ${HUICPP_OUTPUT_LIB}/;
	cp src/*.h ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r src/adaptation ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r src/detail ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r src/inl ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r src/internal ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r src/utils ${HUICPP_OUTPUT_INCLUDE}/;
	cp -r ${OPENSSL_DEP_HOME} ${INSTALL_OUTPUT}/;	
	cp -r ${JSON_DEP_HOME} ${INSTALL_OUTPUT}/;
	
# cp -r ${CURL_DEP_HOME} ${INSTALL_OUTPUT}/;

#	mkdir -p ${INCLUDE_PATH}/;
#	@for dir in ${HUI_ITEMS}; do mkdir -p ${INCLUDE_PATH}/$$dir; done
#	cp src/*.h ${INCLUDE_PATH}/;
#	cp src/*.hpp ${INCLUDE_PATH}/;
#	@for dir in ${HUI_ITEMS}; do cp $$dir/*.h ${INCLUDE_PATH}/$$dir/; done


test:
	@echo ${PRONAME};


