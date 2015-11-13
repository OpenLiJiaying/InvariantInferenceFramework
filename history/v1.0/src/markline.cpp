/*************************************************************************
    > File Name: markline.cpp
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: Thu 17 Sep 2015 02:33:54 PM
 ************************************************************************/
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
using namespace std;
#define MAX 1024
char line[MAX];

/*
 * call format:
 *			./markline filename
 * the filename defines the file we generated from runs of our instrumentated code
 * there are four kinds of lines:
 * + ..... keep them
 * - ..... remove them
 * ? ..... depend on the next + or - or x to decide how to operate them
 * x ..... this should be a bug.  Point |= precondition, but ~|= postcondition
 * 
 * output:
 * the output can be directly give to 'sed' command.
 *
 * return value:
 * returns number of periods of lines which need to be removed.
 * if 0:	nothing to remove
 * if -1:	BUG in original program.
 * if >0:	pass output to 'sed' to do deletion and other operations
 */
int main(int argc, char** argv)
{
	if (argc < 2) 
		return -1;
	ifstream if1(argv[1]);
	int lineNr = 0;
	int startNr = 0;
	char flag[32];
	int label;
	int ret = 0;
	while(!if1.eof()) {
		(if1 >> flag >> label).getline(line, MAX); lineNr++;
		if (flag[0] == '?')
			startNr = lineNr;

		while(flag[0] == '?') {
			if (if1.eof())	goto RET;
			(if1 >> flag >> label).getline(line, MAX); lineNr++;
		}
		
		switch (flag[0]) {
			case '-':
				if (startNr == 0) 
					printf( "-e '%dd' ", lineNr);
				else
					printf("-e '%d,%dd' ", startNr, lineNr);
				ret++;
			case '+':
				startNr = 0;
				break;
			case 'x':
				cerr << endl << "!!Encounter a bug... Find out in file[" << argv[1] << "] at line[" << startNr << "--" << lineNr << "]" << endl;
				return -1;
		}
		flag[0] = ' ';
	}
RET:
	if1.close();
	cout << endl;
//	cout << para << endl;
	return ret;
}
