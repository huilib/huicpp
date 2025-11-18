###
 # @Author: Tom Hui
 # @Date: 2019-12-16 21:03:59
 # @Description: 
 ###
#/bin/bash

vername=$1
now_fold=`pwd`
TRUNK_DIR="../trunk"

# check vername
if [ ! -d ${now_fold}/${vername} ]
then
	echo "input a right version"
	exit 0
fi

if [ -d ${TRUNK_DIR} ]
then
	rm -fr ${TRUNK_DIR}
fi

cp -r ${vername} ${TRUNK_DIR}

if [ ! -d "../release" ]
then 
	mkdir -p ../release
fi

# create release folder
rel_fold=../release/${vername}
if [ -d ${rel_fold} ]
then 
	rm -fr ${rel_fold}
fi


# copy files
cp -r ${now_fold}/${vername} ${rel_fold}
cd ${now_fold}
cp -r  ../deps ${rel_fold}/
cd ${now_fold}/../release
rm -f ${vername}.tar.gz
tar cvfz ${vername}.tar.gz ${vername}
rm -fr ${vername}


exit 0