

include vars.mk

.PHONY: all clean 

NOW_DIR = ${shell pwd}

DIRS = src

all:
	@for dir in ${DIRS}; do make -C $$dir -j${CTNUM}; echo; done 

clean:
	@for dir in ${DIRS}; do make -C $$dir clean; echo; done 