# IIF
A  Invariant Inference Framework using Active Learning and SVMs

## This is IIF project homepage.
Currently, this implementation can support at most 128-int input variables perfectly.
It is safe to  test programs with less than 16 integers now.
For the cast with integer larger than 16, we have not tested.
So be patient, please.




##[Usage]
You should install libsvm first.
You should add {libsvm-root-path} to your path before running the following command.

run the test as following command:
cd $project_root
./test.sh	source.cpp	NumbersOfInputNeed	[RandomedInputLowerBound] [RandomedINputUpperBounder] 

##[Examples]
for example, to test the ./examples/simple/simeple.cpp
we run FIIF as: ./test.sh	examples/simple/simple.cpp 2 -100 100
