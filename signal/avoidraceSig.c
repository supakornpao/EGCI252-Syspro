#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<wait.h>

static void sig_cld(int signo){
	if(signo == SIGUSR1)
		printf("Child\n");
	else if(signo==SIGUSR2)
		printf("Parent\n");
}


int main(){
	pid_t pid;
	int n=0;
	signal(SIGUSR1,sig_cld);
	signal(SIGUSR2,sig_cld);
	
	printf("fork program starting\n");
	pid = fork();
	switch(pid){
		case -1: exit(1);
		case 0:  for(n=0;n<5;n++){
					pause();	
					sleep(1);
					kill(getppid(),SIGUSR1);
				 } 
				 break;
		default: for(n=0;n<5;n++){
					sleep(1);
					kill(pid,SIGUSR2);
					pause();
				 } 
				 break;
	}

	return 0;
}

