/*************************************************************************
  > File Name: tracer.cpp
  > Author: Li Jiaying
  > Mail: lijiaying1989@gmail.com 
  > Created Time: 2015年10月26日 星期一 19时11分46秒
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include "stdio.h"

// child process
void run_target(const char* programname)
{
	printf("\t[%d] target started. Will run '%s'\n", getpid(), programname);

	if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
		perror("ptrace error.");
		return;
	}

	execl(programname, programname, (char*)0);
}


// parent process
void run_debugger(pid_t child_pid)
{
	int wait_status;
	unsigned icounter = 0;
	printf("[%d] debugger started:\n", getpid());

	wait(&wait_status);

	while(WIFSTOPPED(wait_status)) {
		icounter++;
		struct user_regs_struct regs;
		unsigned long long instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);
		printf ("[%d] icounter = %u. EIP = 0x%016llx,  instr = 0x%016llx\n", getpid(), icounter, regs.rip, instr);
		if(ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
			printf("ptrace");
			return;
		} 
		wait(&wait_status);
	}
	printf("[%d] the child executered %u instructions\n", getpid(), icounter);
}


int main(int argc, char** argv)
{
	pid_t child_pid;

	if (argc < 2) {
		fprintf(stderr, "Expected a program name as argument\n");
		return -1;

	}

	child_pid = fork();
	if (child_pid == 0)
		run_target(argv[1]);
	else if (child_pid > 0)
		run_debugger(child_pid);
	else {
		perror("fork");
		return -1;
	}

	return 0;
}
