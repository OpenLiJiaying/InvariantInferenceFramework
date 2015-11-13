/*************************************************************************
    > File Name: existencecheck.cpp
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: Fri 18 Sep 2015 04:30:09 PM
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#define LINE 1024
#define SZ 16

int main(int argc, char** argv)
{
	if (argc < 3)
		return -1;
	ifstream ifRepo(argv[1]);
	ifstream ifCand(argv[2]);
	int lineNr, size;
	
	ifCand >> lineNr >> size;
	double cand[LINE][SZ];

	for (int i = 0; i < lineNr; i++) {
		for (int j = 0; j < size; j++) 
			ifCand >> cand[i][j];
	}
	int tempint;
	char tempchar;
	double record[SZ];

	while(!ifRepo.eof()) {
		cout << "@";
		ifRepo >> tempchar;
		for (int i = 0; i < size; i++) {
			ifRepo >> tempint >> tempchar >> record[i]; 
		}
		for (int i = 0; i < lineNr; i++) {
			for (int j = 0; j < size; j++)
				if (cand[i][j] == record[j]) 
					return -1;
		}
	}
	ifRepo.close();
	ifCand.close();
	return 0;
}
