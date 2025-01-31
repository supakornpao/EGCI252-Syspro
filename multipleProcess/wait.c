#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main(){
	pid_t pid; char* msg; int n;

	printf("fork program starting\n");

	pid = fork();
	switch(pid){
		case -1: _exit(1);
		case 0: msg = "Child!\n";n=3; break;
		default: msg = "Parent!\n"; n=15; break;
	}

	for(;n>0;n--){puts(msg); sleep(1);}

	if(pid){//if pid != 0
		int stat_val; pid_t child_pid;

		child_pid = wait(&stat_val);
		printf("Child has finished: PID = %d\n",child_pid);
		if(WIFEXITED(stat_val))
			printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
		else
			printf("Child termintated abnormally\n");
	
	}

	_exit(0);
	
}
