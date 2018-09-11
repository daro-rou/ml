#!/bin/bash


CMAKE_DIR=`pwd`/../cmake-3.12.0-Linux-x86_64/bin
echo CMAKE_DIR=$CMAKE_DIR
BUILD_DIR=$(pwd)/build
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
cmake .. -DUSE_SSE2_INSTRUCTIONS=ON | tee cmake-generate.log
cmake --build . --config Release | tee cmake-build.log

