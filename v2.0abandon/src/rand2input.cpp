/*************************************************************************
    > File Name: rand2input.cpp
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: Thu Sep 24 02:48:56 2015
 ************************************************************************/

#include <iostream>
using namespace std;
#define MAX 1024
char s[MAX];

int main()
{
	bool bprint=false;
	cin.getline(s, MAX);
//	cout << "rand2input: " << s << endl;
	for (int i = 0; s[i] != '\0'; i++) {
		char c = s[i];
		if (c == ':')
			bprint = true;
		else if (bprint) {
		if (c == ' ' || c == '\t')
			bprint = false;
		cout << c;
		}
	}
	cout << endl;
	return 0;
}
