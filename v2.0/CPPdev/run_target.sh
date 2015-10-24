#!/bin/bash

if [ $# -ge 2 ]
then
	target_exe=$1
	input_file=$2
else
	echo "./test.sh needs more parameters"
	echo "./test.sh test_cpp_pathname input_num [MIN] [MAX]"
	echo "try it again..."
	exit 1
fi


echo "************************************** 15 **************************************************************"
mkdir -p ./data

#############################################################################################################################################
######################################## REAL LEARNING STARTS HERE ##########################################################################
#############################################################################################################################################

cond=0

K=1
echo "************************************** start **************************************************************"
echo -n " ..."


#############################################################################################################################################
# run the program with all the samples
while IFS='' read -r line || [[ -n "$line" ]]; do
	echo "$line" |xargs  $target_exe >> "./data/train1_"${K}
done < $input_file 


#############################################################################################################################################
# do the marking for generated data
#############################################################################################################################################
echo "************************************** 39 **************************************************************"
./markline  "./data/train1_"${K} > "./data/tempmark_"${K}	## do the marking for awk command..  output like -e '3,5d' -e '7,9d' 
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
	cat "./data/tempmark_"${K} | xargs sed "data/train1_"${K} >  "./data/train2_"${K}
	awk '{$1="";print $0}'  "./data/train2_"${K} | cut -c2- >> ./data/train
fi
# copy the data for plot use... for matlab
cat ./data/train > "./data/points"${K} 

echo "************************************** 62 **************************************************************"


rm ./data/train1_*
rm ./data/train2_*
echo "*************************************  end  *************************************************************"
exit 0

