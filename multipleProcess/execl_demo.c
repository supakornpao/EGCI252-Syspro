#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	pid_t pid = fork();
	printf("Process ID: %d\n",(int)pid);
	if(pid == 0)
		execl("./print","abc","1","10",NULL);//child process
	else
		execl("./print","abc","2","20",NULL);//parent process

}
