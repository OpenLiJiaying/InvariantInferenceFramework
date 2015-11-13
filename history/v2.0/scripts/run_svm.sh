#!/bin/bash

#cat ./data/train 
DOUBLE_MAX=1.79769e+308
cd ./data
#echo "------------------- start to run SVM ----------------------"
echo -n "...svm..."
svm-train  -q -t 0 -c $DOUBLE_MAX train
svm-predict -q train train.model train.predict
#echo "------------------ finish running SVM ---------------------"
#echo "------------ finish running SVM ----------"
##echo 
echo -n "" > svmline
../bin/svm2plane -m train.model -t train -p svmline
cond=$?
if [ $cond -eq 254 ]
then
	cd ..
	echo -n -e "\b\b\b\b\b\b\b\b\b\b"
	continue
fi

cat svmline >> lines
cd ..


exit 0
