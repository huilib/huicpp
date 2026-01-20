
include ../vars.mk

.PHONY: all clean 

HUICPP_BASE=${NOW_DIR}/../../../deps

HUICPP_INCLUDE_PATH=${HUICPP_BASE}/${TEST_PLATCODE}/huicpp/include
HUICPP_LIB_PATH=${HUICPP_BASE}/${TEST_PLATCODE}/huicpp/lib

all:
	g++ -I${HUICPP_INCLUDE_PATH} main.cpp detectxbapp.cpp -L${HUICPP_LIB_PATH} -lhuicpp -pthread  -o ${TARGET}

clean:
	rm -f *.o
	rm -f ${TARGET}