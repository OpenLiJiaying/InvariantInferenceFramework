#!/bin/bash

if [ $# -ge 2 ]
then
	test_cpp=$1
	test_out=$2
else
	echo "./test.sh needs more parameters"
	echo "./test.sh test_cpp_pathname"
	echo "try it again..."
	exit 1
fi


pwd=`pwd` 
mkdir -p ./tmpinclude


echo "#include <stdio.h>
#include <stdlib.h>

int _passP = 0;
int _passQ = 0;
int _t_condition = 0;

#undef assert
#ifdef NDEBUG
#define assert(expr)  ((void) 0)
#else
#define assert(expr) do { \\
if (_t_condition == 0) {\\
	_passP = (expr)? 1 : 0;\\
} else if (_t_condition == 1) { \\
_passQ = (expr)? 1 : 0;\\
if (_passP && _passQ) printf (\"+ 1 \");\\
else if (_passP && !_passQ) { \\
		printf (\"x 1 \");\\
		fprintf(stderr, \"encounter a counter-example. assertion failed: %s, file %s, line %i\n\", \\
		#expr, __FILE__, __LINE__);\\
	exit(2);\\
	}\\
else if (!_passP && _passQ) printf (\"- -1 \");\\
else printf (\"+ -1 \");\\
} else {\\
if(!(expr)) \\
	fprintf(stderr, \"Assertion failed: %s, file %s, line %i\n\", \\
	#expr, __FILE__, __LINE__);\\
}\\
_t_condition++;\\
} while (0)
#endif" > ./tmpinclude/assert.h 



g++ -I ./tmpinclude  $test_cpp -o $test_out
if [ $? -ne 0 ]
then
	rm -rf ./tmpinclude
	echo "can not compile $test_cpp correctly, check your file..."
	exit -1
fi

make > ./makeout
if [ $? -ne 0 ]
then
	cat ./makeout
	echo "make command encounter errors..."
	echo "stop here."
	rm ./makeout
	exit -1
fi


rm -rf ./tmpinclude
rm ./makeout
exit 0
