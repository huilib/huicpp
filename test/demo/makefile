

include vars.mk

.PHONY: all clean 


DIRS = src

all:
	@for dir in ${DIRS}; do make -C $$dir -j${CTNUM}; echo; done 

clean:
	@for dir in ${DIRS}; do make -C $$dir clean; echo; done 