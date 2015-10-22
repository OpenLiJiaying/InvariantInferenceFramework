#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <stdarg.h>
#include <unistd.h>
#include "iif.h"
#include "tuple.h"
using namespace std;

int main(int argc, char** argv, char** envp)
{
	std::unordered_map<std::size_t, Tuple> map;

	string testfile_path;
	srand(time(NULL));
	if (argc >= 3) {
		testfile_path = argv[1];
		dim = atoi(argv[2]);
		num_tests_begin	= 4 * dim;
		num_tests_after = 4 * dim;
	} else {
		cout << "IIF needs more parameters." << endl;
		cout << "executable testfile_path input_num [MIN] [MAX]" << endl;
		exit(-1);
	}
	if (argc >= 5) {
		mini = atoi(argv[3]);
		maxi = atoi(argv[4]);
	}
	
	string testfile_out = "./target.out";
	const char* str_make_testfile = (string("g++ ") +  testfile_path + string(" -o ") + testfile_out).c_str();
	cout << str_make_testfile << endl;
	system(str_make_testfile);

	for (int k = 1; k < num_iters; k++) {
		if (k == 1) {	// the first round
			auto m = Tuple::rand_gen();
			map.insert(make_pair(m->hash(), *m));
			const char* str_run_target = (string(testfile_out) + " " + m->to_string()).c_str();
			cout << "str to run target: " << str_run_target << endl;
			system(str_run_target);
			
//			const char* str_randgen = (string("./bin/randgen ") + to_string(dim) + " " + to_string(num_tests_begin) + 
//					" " + to_string(min) + " " + to_string(max)).c_str();
//			system(str_randgen);
		}
		cout << "other things to do " << k << endl;
	}



/*
	cout << sizeof(std::size_t) << endl;
	auto k1 = Tuple::make_tuple(12, 16, 205);
	Tuple k2 = Tuple::make_tuple(2, 341, 212);
	std::unordered_map<std::size_t, Tuple> map;
	map.insert(make_pair(k1.hash(), k1));
	map.insert(make_pair(k2.hash(), k2));
	for (auto& x: map)
		cout << x.first << ":" << x.second.s[0] << ", " << x.second.s[1] << ", " << x.second.s[2] << endl;

	cout << endl;
	auto m1 = map.find(k1.hash());
	auto m2 = map.find(Tuple::make_tuple(12, 16, 203).hash());
	if (m1 == map.end()) 
		cout << "not found" << endl;
	else
		cout << "m1 found" << m1->first << ":" << m1->second.s[0] << ", " << m1->second.s[1] << ", " << m1->second.s[2] << endl;
	if (m2 == map.end()) 
		cout << "not found" << endl;
	else
		cout << "m2 found" << m2->first << ":" << m2->second.s[0] << ", " << m2->second.s[1] << ", " << m2->second.s[2] << endl;
*/
	return 0;
}
