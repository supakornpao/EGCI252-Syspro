#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<signal.h>

#define FIFO_1 "./fifo1to2"
#define FIFO_2 "./fifo2to1"
#define MAX_RBUF 80
int FIFO_FD1, FIFO_FD2;

pid_t other_user_pid = -1;

void sighandler(int signal){
	if(signal == SIGUSR1) printf("User 1 terminated\n");
	else if(signal == SIGUSR2) printf("User 2 terminated\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	int child, nbytes;
	char rbuf[MAX_RBUF] = "";
	pid_t my_pid = getpid();
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
	
	argv++;
	if (strcmp(*argv, "1") == 0){
		signal(SIGUSR1, sighandler);
		FIFO_FD1 = open(FIFO_1, O_WRONLY);
		FIFO_FD2 = open(FIFO_2, O_RDONLY);
		write(FIFO_FD1,&my_pid, sizeof(my_pid));
		read(FIFO_FD2, &other_user_pid, sizeof(my_pid));
		child = fork();
		switch (child){
			case -1 : perror("Forking failed"); exit(EXIT_FAILURE);
			case 0 : while (strncmp(rbuf, "end chat", 8)){
						memset(rbuf, 0, MAX_RBUF);
						nbytes = read(FIFO_FD2,rbuf,MAX_RBUF);
						if(nbytes>0)
							printf(">> %s",rbuf);
					}
					kill(getppid(), SIGUSR1);  
					exit(EXIT_SUCCESS);
					

			default : while (strncmp(rbuf, "end chat", 8)){
									memset(rbuf, 0, MAX_RBUF);
									fgets(rbuf,MAX_RBUF,stdin);
									write(FIFO_FD1,rbuf,strlen(rbuf)+1);
								}
								if (other_user_pid != -1) {
									kill(other_user_pid, SIGUSR2);
								}
								kill(child, SIGUSR1);
								exit(EXIT_SUCCESS);
								break;
		}
	}
	else if (strcmp(*argv, "2") == 0){

		signal(SIGUSR2, sighandler);
		FIFO_FD1 = open(FIFO_1, O_RDONLY);
		FIFO_FD2 = open(FIFO_2, O_WRONLY);
		read(FIFO_FD1, &other_user_pid, sizeof(my_pid));
		write(FIFO_FD2,&my_pid, sizeof(my_pid));
		child = fork();
		switch (child){
		case -1 : perror("Forking failed"); exit(EXIT_FAILURE);
		case 0 : while (strncmp(rbuf, "end chat", 8)){
								memset(rbuf, 0, MAX_RBUF);
								nbytes = read(FIFO_FD1,rbuf,MAX_RBUF);
								if(nbytes>0)
									printf(">> %s",rbuf);
						}
						kill(getppid(), SIGUSR2);
						exit(EXIT_SUCCESS);
						
		default : while (strncmp(rbuf, "end chat", 8))
				{
								memset(rbuf, 0, MAX_RBUF);
								fgets(rbuf, MAX_RBUF,stdin);
								write(FIFO_FD2,rbuf,strlen(rbuf)+1);
				}
				if (other_user_pid != -1) {
					kill(other_user_pid, SIGUSR1);
				}
				kill(child, SIGUSR2);
				exit(EXIT_SUCCESS);
				break;
		}
	}
////////////////////////////////////////////////////
	if (FIFO_FD1 != -1) close(FIFO_FD1);
	if (FIFO_FD2 != -1) close(FIFO_FD2);
	exit(EXIT_SUCCESS);
}
