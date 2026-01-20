##===================================================================##
## File Name: makefile
## Create Author: Joseph Hui
## Create Date: Tue Sep 12 1359 2015
## Description:
##		
##===================================================================##

include ../inc.mk


TARGET = ${LIB_PATH}/libhuicpp.a

SUB_SRCS = adaptation baseinside detail detail/log inl internal utils process systemcall image

ifeq "$(OS_TYPE)" "Darwin"
LINK_CC_LIBS += -liconv
endif

all: def

def: ${TARGET}


include ../route.mk
