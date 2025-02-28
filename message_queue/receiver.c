#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct a_msg{
	long int msg_type;
	char data[BUFSIZ];
};

int main(void){
	int running = 1, msgID;
	struct a_msg a_msg;
	long int rcv_msg_type = 0;

	msgID = msgget((key_t)1234, 0666|IPC_CREAT);
	if(msgID == -1){
		fprintf(stderr, "msgget failed\n");
		exit(EXIT_FAILURE);
	}

	while(running){
		if(msgrcv(msgID,(void*)&a_msg,BUFSIZ,rcv_msg_type,0)==-1){
			fprintf(stderr,"msgrcv failed\n");
			exit(EXIT_FAILURE);
		}
		printf("Received Message: %s\n",a_msg.data);
		if(strncmp(a_msg.data,"end",3)==0)
			running = 0;
	}
	if(msgctl(msgID, IPC_RMID, 0)==-1){
		fprintf(stderr, "msgctl failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
