/*************************************************************************
  > File Name: test_shell.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月15日 星期四 13时29分26秒
 ************************************************************************/
#include <cstdlib>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define MAXDIM  16
static int key_length = 2;

struct Key {
	int s[MAXDIM];
	bool operator==(const Key &key2) const {
		for (int i = 0; i < key_length; i++) 
			if (s[i] != key2.s[i])
				return false;
		return true;
	}
};

template <class T>
class MyHash;

template<>
class MyHash<Key>
{
	public:
		std::size_t operator()(const Key& k) const {
			std::size_t t = 0;
			for (int i = 0; i < key_length; i++) {
				t = t << 4 | k.s[i];
			}
			return t;
		}
};




int main(int argc, char** argv, char** envp)
{
	key_length = 3;
	Key k1, k2;
	k1.s[0] = 12;
	k1.s[1] = 16;
	k1.s[2] = 205;
	k2.s[0] = 2;
	k2.s[1] = 341;
	k2.s[2] = 212;
//	std::hash<Key> hash_fn;
	cout << "hash(k1) = " << MyHash<Key>()(k1) << endl; 
	cout << "hash(k2) = " << MyHash<Key>()(k2) << endl; 
	std::unordered_map<int, Key>

	/*
	pid_t pid;
	int status;

	pid = fork();
	if (pid > 0) {
		waitpid(pid, &status, 0);
	} else {
		execve("./test.sh", argv, envp);
	}

	cout << "exit value is " << status << endl;
	*/
	return 0;
}
