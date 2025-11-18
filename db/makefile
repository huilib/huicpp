##===================================================================##
## File Name: makefile
## Create Author: Tom Hui
## Create Date: Thu Dec 20 129 2017
## Description:
## apt-get install msqlpp;
## apt-get install libmysqlclient-dev;
##===================================================================##


include ../inc.mk

TARGET = ${LIB_PATH}/libhuidb.a

SRC = $(wildcard *.cpp)

OBJS = ${patsubst %.cpp, %.o, ${SRC}}


all: def

def: ${TARGET}


include ../route.mk
