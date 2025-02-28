#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/msg.h>

#define FIFO_1 "./fifo1to2"
#define FIFO_2 "./fifo2to1"
#define MAX_RBUF 80
int FIFO_FD1, FIFO_FD2;

struct msg{
	long int msg_type;
	char data[BUFSIZ];
};


int main(int argc, char *argv[])
{
	int child, nbytes;
	char rbuf[MAX_RBUF] = "";
	if (argc != 2){	
		fprintf(stderr, "Usage: %s <[1, 2]>\n", *argv);
		exit(EXIT_FAILURE);
	}
	if (access(FIFO_1, F_OK) == -1){
		FIFO_FD1 = mkfifo(FIFO_1, 0777);
		if (FIFO_FD1==-1){
			fprintf(stderr, "Could not create fifo %s\n", FIFO_1);
			exit(EXIT_FAILURE);
		}
	}
	if (access(FIFO_2, F_OK) == -1){
		FIFO_FD2 = mkfifo(FIFO_2, 0777);
		if (FIFO_FD2==-1){
			fprintf(stderr, "Could not create fifo %s\n", FIFO_2);
			exit(EXIT_FAILURE);
		}
	}
////////////////////////////////////////////////////
	FIFO_FD1 = open(FIFO_1, O_WRONLY);
	FIFO_FD2 = open(FIFO_2, O_RDONLY);
	argv++;
	if (strcmp(*argv, "1") == 0){
		
		child = fork();
		switch (child){
			case -1 : perror("Forking failed"); exit(EXIT_FAILURE);
			case 0 : while (strncmp(rbuf, "end chat", 8)!=0){
//////////////// Put your code here ////////////////						msgID = msgi

						read(FIFO_FD2,rbuf, sizeof(rbuf));
						write(1,rbuf,sizeof(rbuf));

					}
					break;

			default : while (strncmp(rbuf, "end chat", 8)!=0){
						struct msg amsg;
						char buffer[MAX_RBUF];
						printf("Enter a message: ");
						fflush(stdout);
						int msgID = msgget((key_t)1234,0666|IPC_CREAT);
						int running = 1;
						
						read(0, buffer, MAX_RBUF);
						amsg.msg_type = 0;
						strcpy(amsg.data, buffer);
						if(msgsnd(msgID,(void*)&amsg,MAX_RBUF,0)==-1)
							printf("msg send fails");
						
//////////////// Put your code here ////////////////
					}
					break;
		}
	}
	else if (strcmp(*argv, "2") == 0){
		child = fork();
		switch (child){
		case -1 : perror("Forking failed"); exit(EXIT_FAILURE);
		case 0 : while (strncmp(rbuf, "end chat", 8)){
//////////////// Put your code here ////////////////
				}
				break;
		default : while (strncmp(rbuf, "end chat", 8))
				{
//////////////// Put your code here ////////////////
				}
		}
	}
////////////////////////////////////////////////////
	if (FIFO_FD1 != -1) close(FIFO_FD1);
	if (FIFO_FD2 != -1) close(FIFO_FD2);
	exit(EXIT_SUCCESS);
}
