#!/bin/bash

#. /home/dan/usr/bin/javaenv
#/home/dan/usr/eclipse-jee/eclipse/eclipse -vmargs -Dosgi.locking=none
CMAKE_DIR=/home/dan/cmake-3.12.0-Linux-x86_64/bin
BUILD_DIR=$(pwd)/tools/imglab/build
export PATH=${CMAKE_DIR}:$PATH
#mkdir build
#cd make
if [  -d "${BUILD_DIR}" ]; 
then 
	echo Warning folder ${BUILD_DIR} exists 
	#rm -r ${BUILD_DIR}
else
	mkdir "${BUILD_DIR}"
fi
cd "${BUILD_DIR}"
#cmake --help
cmake ..  -DUSE_SSE2_INSTRUCTIONS=ON | tee cmake-generate.log
cmake --build . --config Release | tee cmake-build.log

