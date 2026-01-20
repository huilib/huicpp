

.PHONY: all clean 

NOW_DIR = ${shell pwd}
TEST_PLATCODE=linux64

TARGET=argapptest

HUICPP_BASE=${NOW_DIR}/../../deps/${TEST_PLATCODE}/huicpp
HUICPP_INCLUDE_PATH=${HUICPP_BASE}/include
HUICPP_LIB_PATH=${HUICPP_BASE}/lib

all:
	g++ -I${HUICPP_INCLUDE_PATH} apptest.cpp testapp.cpp -L${HUICPP_LIB_PATH} -lhuicpp -pthread  -o ${TARGET}

clean:
	rm -f *.o
	rm -f ${TARGET}