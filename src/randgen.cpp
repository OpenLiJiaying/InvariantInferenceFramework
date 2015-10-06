#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#define MAX 10240
#define SZ 32

int record[SZ];
int result[SZ];
int size = 2;
char line[MAX];

int checkduplicate(char* filename)
{
	return 0;
	// not use now.
	ifstream ifRepo(filename);

	int tempint;
	char tempchar;
	double record[SZ];

	while(!ifRepo.eof()) {
		cout << "-";
		ifRepo >> tempchar;
		for (int i = 0; i < size; i++) {
			ifRepo >> tempint >> tempchar >> record[i]; 
			if (record[i] != result[i]) {
				if (i == size - 1)
					ifRepo.getline(line, MAX);
				break;
			}
		}
		ifRepo.close();
		return 1;
	}
	ifRepo.close();
	return 0;
}

/*
 * mian function:
 * call format:
 *				randgen size num start end
 * size:	number of random number in each line.
 *			default 1: each number per line.
 * num:		how many lines to generate
 *			default 1: only generate 1 line
 * start:	defines the minimal value of random numbers
 * end:		defines the maximal value of random numbers
 * 
 */
int main (int argc, char** argv) {
	int seed = time(NULL);
	int size = 1;
	int start = 0, end = 100;
	int num = 1;
	switch (argc) {
		case 6:
		case 5:
			end = atoi(argv[4]);
		case 4:
			start = atoi(argv[3]);
		case 3:
			num = atoi(argv[2]);
		case 2:
			size = atoi(argv[1]);
			break;
	}

	srand(seed);

	int length = end - start + 1;
	for (int i = 0; i < num; i++) {
		do {
			for (int j = 0; j < size; j++)
				result[j] = rand() % length + start;
		} while (checkduplicate(NULL));

		for (int j = 0; j < size; j++)
			cout <<  j+1 << ":" <<result[j] << " ";
		cout << endl;
	}

	return 0;
}

