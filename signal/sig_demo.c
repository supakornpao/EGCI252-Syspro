#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void my_alarm(int sig){
	for(int i=0; i<5; i++){
		printf("Alarm!!\n");
	}
}

int main(void) {
	int pid;
	printf("Alarm clock is starting...\n");

	if((pid = fork())==0){
		sleep(3);
		kill(getppid(),SIGALRM);//send signal SIGALRM
		exit(0);
	}
	printf("Waiting for alarm..\n");
	(void)signal(SIGALRM,my_alarm); //how to handle when receive signal
	pause();
	printf("Done\n");
	exit(0);
}
