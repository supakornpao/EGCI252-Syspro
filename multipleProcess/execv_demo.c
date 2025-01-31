#include<stdio.h>
#include<unistd.h>

int main(){
	char* argv1[] = {"print","1","10",NULL};
	char* argv2[] = {"print","2","20",NULL};
	pid_t pid = fork();

	printf("Process ID: %d\n",pid);

	if(pid == 0)
		execv("./print",argv1);
	else
		execv("./print",argv2);
}
