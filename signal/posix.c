#include<signal.h>

#include<stdio.h>
#include<unistd.h>

void rx_int(int sig){
	printf("SIGINT - %d Received!!\n",sig);
}

int main(void){
	struct sigaction act;

	act.sa_handler = rx_int;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT,&act,NULL);
	while(1){
		printf("Running...\n");
		sleep(1);
	}
}
