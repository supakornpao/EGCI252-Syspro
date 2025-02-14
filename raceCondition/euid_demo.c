#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]){
	int pid = fork();

	if(pid == 0){
		printf("Child : RUID = %d, EUID = %d\n",getuid(),geteuid());
		execlp(argv[1],argv[2],NULL);
	}
	else{
		wait(NULL);
		printf("Parent : RUID = %d, EUID = %d",getuid(),geteuid());
	}
}
