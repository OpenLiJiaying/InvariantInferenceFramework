#!/bin/bash

if [ $# -ge 1 ]
then
	file_path=$1
else
	exit 1
fi


test_cpp=${file_path}
test_out="./target.out"


mkdir -p ./data
rm -rf ./data/*
mkdir -p ./bin

cd ..
make # > ./makeout
cd scripts

if [ $? -ne 0 ]
then
#	cat ./makeout
	echo "make command encounter errors..."
	echo "stop here."
#	rm ./makeout
	exit -1
fi
#rm ./makeout
g++ $test_cpp -o $test_out
