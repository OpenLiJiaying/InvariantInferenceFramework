# An  Invariant Inference Framework using Active Learning and SVMs


This is the homepage of our iif project of the paper [An Invariant Inference Framework by
Active Learning and SVMs](http://lijiaying.github.io/Papers/AInvariantInferenceFrameworkbyActiveLearningandSVMs.pdf) by Li Jiaying.

For you to run the experiments on your own machine, please follow the steps below to set up your experiment environment.

## Build Invariant Inference Framework
To build the framework currently is very easy,
there is not much dependencies you need to satisfy before build the whole project.

Dependencies:
* [libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) remember to put {libsvm}/bin folder into $PATH
* [klee](https://klee.github.io/) currently this is not essential.


### For Windows Users:
* Windows xp or later
* Visual Studio 2015
```
Create a new Win32 console project in VS2015
Drag the header files in {include} folder to Header Files
Drag other Cpp files in {src} and {example} folder to Source Files
You can directly build and run the project as you want.
```

## For Linux Users:
* cmake 2.8 or later
```
git clone git@github.com:lijiaying/InvariantInferenceFramework.git
cd InvariantInferenceFramework
cd test
mkdir build
cd build
cmake ..
make
./learn
```

## Add new tests to test our framework
#####As InvariantInferenceFramework is integrated with your examples, you need to do some modification on source code level before you can test your examples.
* rewrite your loop code in a function with the name you like, my_loop_example for instance
* put the function in {examples/examples.cpp} or create a new file contains the function and remember to include "../include/header.h"
* edit file {include/config.h}, add "define my_loop_example m", and "#define VARS (the number of parameters in your loop)"
* make your project and then run the executable file
