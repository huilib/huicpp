#!/bin/bash

dirs=`ls`

for dd in ${dirs}
do 
    ## make sure it is a directory. 
    ## in case 'build_test.sh'
    if [ ! -d ${dd} ]
    then 
        continue
    fi 
    
    pushd ${dd}
        ## make sure there is a makefile in this directory.
        if [ ! -f "makefile" ]
        then 
            continue 
        fi

        echo "build test object ${dd}"
        make clean
        make
    popd

done 
