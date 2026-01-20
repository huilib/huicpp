
include ../vars.mk

.PHONY: all clean 

NOW_DIR = ${shell pwd}

HUICPP_INCLUDE_PATH=${HUICPP_BASE}/${TEST_PLATCODE}/huicpp/include
HUICPP_LIB_PATH=${HUICPP_BASE}/${TEST_PLATCODE}/huicpp/lib


all:
	g++ -I${HUICPP_INCLUDE_PATH} main.cpp detectxbapp.cpp -L${HUICPP_LIB_PATH} -lhuicpp -pthread -g	\
		-I${OPENCV4_INCLUDE_PATH} -fPIC -lopencv_core -lopencv_videoio   -o ${TARGET}

clean:
	rm -f *.o
	rm -f ${TARGET}