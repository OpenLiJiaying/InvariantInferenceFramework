#!/bin/bash

if [ $# -ge 2 ]
then
	test_out=$1
	input_num=$2
else
	echo "./test.sh test_cpp_pathname input_num [MIN] [MAX]"
	echo "try it again..."
	exit 1
fi

if [ $# -ge 4 ] 
then
	MIN=$3
	MAX=$4
else
	MIN=-100
	MAX=100
fi



NUM_ITER_BEGIN=`expr 4 \* $input_num`
cond=0
prev=0 ## number of previous test cases
cur=0 ## number of cur run test cases after deletion
numfail=0 ###number of failed tests run...
rm -rf ./data/*

echo -n "" > ./data/points
echo -n "" > ./data/lines

echo "************************************** start **************************************************************"
echo -n " ..."
K=1 

echo -n "" > ./data/train
./bin/randgen $input_num $NUM_ITER_BEGIN $MIN $MAX > ./data/sample1

echo -n $K


#############################################################################################################################################
# run the program with all the samples
echo -n "" >  ./data/train1_1
while IFS='' read -r line || [[ -n "$line" ]]; do
	echo "$line" | ./bin/rand2input > ./data/input
	if [ $? -eq 2 ]
	then
		echo -n "The input is: "
		echo "$line" | ./bin/rand2input
		exit -1;
	fi
	cat ./data/input |xargs $test_out >> ./data/train1_1
done < ./data/sample1 


#############################################################################################################################################
# do the marking for generated data
#############################################################################################################################################
./bin/markline  ./data/train1_1 > ./data/tempmark_1	## do the marking for awk command..  output like -e '3,5d' -e '7,9d' 
cond=$?


#############################################################################################################################################
# cut and convert the output file to the standard svm input data format
# cond == 255[-1]  => there is '!' symbol. which means a bug in program. m0 |= Precondition, but mn |=\ Postcondition
#############################################################################################################################################
if [ $cond -eq 255 ]
then
	echo "COUNTER EXAMPLE! ./bin/markline exit with -1."
	exit -1
elif [ $cond -eq 0 ]		## there is no line need to delete...
then						## echo "NO LINE TO REMOVE!"
	awk '{$1="";print $0}'  "./data/train1_"${K} | cut -c2- >>  ./data/train
else					## we need to delete some lines, which have been marked by above output
	cat ./data/tempmark_1 | xargs sed data/train1_1 >  ./data/train2_1
	awk '{$1="";print $0}'  ./data/train2_1 | cut -c2- >> ./data/train
fi
# copy the data for plot use... for matlab
cat ./data/train > ./data/points1

./run_svm.sh


rm -f $test_out
exit 0
