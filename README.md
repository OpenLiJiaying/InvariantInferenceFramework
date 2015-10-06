# FIIF
A Fast Invariant Inference Framework using Active Learning and SVMs

## This is FIIF project homepage.
Currently, this implementation can support 2-int input variables perfectly.
We will test programs with 3-int or more inputs in the coming future.
Be patient, please.




##[Usage]
You should add {libsvm-root-path} to your path before running the following command.

run the test as following command:
cd $project_root
./test.sh	source.cpp	NumbersOfInputNeed	[RandomedInputLowerBound] [RandomedINputUpperBounder] 

##[Examples]
for example, to test the ./examples/simple/simeple.cpp
we run FIIF as: ./test.sh	examples/simple/simple.cpp 2 -100 100
