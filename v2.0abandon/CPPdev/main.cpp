#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_DIM  16
#define MAX_BIT  32

int dim = 2;
int mini = -100;
int maxi = 100;
int num_tests_begin = 16;
int num_tests_after = 4;
int num_iters = 32;


class Tuple {
	public:
	static Tuple* make_tuple(int first, ...) {
		Tuple* tmp = new Tuple();
		va_list ap;
		va_start(ap, first);
		tmp->s[0] = first;
		std::cout << tmp->s[0] << "==";
		for (int i = 1; i < dim; i++) {
			tmp->s[i] = va_arg(ap, int);
			std::cout << tmp->s[i] << "==";
		}
		std::cout << std::endl;
		return tmp;
	}

	int s[MAX_DIM];
	std::size_t hash(){
		std::size_t t = 0;
		for (int i = 0; i < dim; i++) {
			t = t << 4 | s[i];
		}
		return t;
	}
	static Tuple* rand_gen()
	{
		int length = maxi - mini + 1;
		Tuple* t = new Tuple();
		for (int i = 0; i < dim; i++)
			t->s[i] = rand() % length + mini;
		return t;
	}
	std::string to_string()
	{
		std::string str = "";
		for (int i = 0; i < dim; i++) 
			str += std::to_string(s[i]) + " ";
		str += "\0";
		return str;
	}
};









int main(int argc, char** argv, char** envp)
{
	/* 
	 * parsing arguments
	 */
	std::string testfile_path;
	if (argc >= 3) {
		testfile_path = argv[1];
		dim = atoi(argv[2]);
		num_tests_begin	= 4 * dim;
		num_tests_after = 4 * dim;
	} else {
		std::cout << "IIF needs more parameters." << std::endl;
		std::cout << "executable testfile_path input_num [MIN] [MAX]" << std::endl;
		exit(-1);
	}
	if (argc >= 5) {
		mini = atoi(argv[3]);
		maxi = atoi(argv[4]);
	}
	


	/*
	 * Run target program...
	 */
	std::string testfile_out = "./target.out";
	const char* str_make_testfile = (std::string("g++ ") +  testfile_path + std::string(" -o ") + testfile_out).c_str();
	std::cout << str_make_testfile << std::endl;
	system(str_make_testfile);

	std::unordered_map<std::size_t, Tuple> map;
	srand(time(NULL));
	for (int k = 1; k < num_iters; k++) {
		if (k == 1) {	// the first round
			auto m = Tuple::rand_gen();
			map.insert(std::make_pair(m->hash(), *m));
			const char* str_run_target = (testfile_out + " " + m->to_string()).c_str();
			std::cout << "str to run target: " << str_run_target << std::endl;
			system(str_run_target);
			
//			const char* str_randgen = (string("./bin/randgen ") + to_string(dim) + " " + to_string(num_tests_begin) + 
//					" " + to_string(min) + " " + to_string(max)).c_str();
//			system(str_randgen);
		}
		std::cout << "other things to do " << k << std::endl;
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
