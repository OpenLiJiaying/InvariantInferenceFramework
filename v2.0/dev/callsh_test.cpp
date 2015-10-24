/*************************************************************************
    > File Name: test_shell.cpp
    > Author: Li Jiaying
    > Mail: lijiaying1989@gmail.com 
    > Created Time: 2015年10月15日 星期四 13时29分26秒
 ************************************************************************/
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char** argv, char** envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid > 0) {
		waitpid(pid, &status, 0);
	} else {
		execve("./test.sh", argv, envp);
	}

/*
	int ret = system("ls -al /etc/passwd /etc/shadow");
	cout << "exit value is " << ret << endl;
	ret = system("cd FIIF; ./test.sh ./examples/ins2.cpp 2");
	ret = system("cd FIIF; ./test.sh", argv);
*/
	cout << "exit value is " << status << endl;
	return 0;
}
