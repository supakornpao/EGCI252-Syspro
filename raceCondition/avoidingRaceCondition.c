#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<wait.h>

int main(){
	pid_t pid; char* msg,*omsg,c;
	int n,turn = 0, flagfd;

	flagfd = open("flag",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	write(flagfd,&turn,1);
	printf("fork program starting\n");
	pid = fork();
	switch(pid){
		case -1: exit(1);
		case 0: msg = "Child!\n"; n = 5; break;
		default: msg = "Parent!\n"; n=5; break;
	}

	setbuf(stdout, NULL);

	for(;n>0;n--){
		if(pid){
			while(turn){
				sleep(1);
				lseek(flagfd, 0,SEEK_SET);
				read(flagfd, &turn,1);
			}
		}
		else{
			while(!turn){
				sleep(1);
				lseek(flagfd, 0, SEEK_SET);
				read(flagfd, &turn,1);
			}
		}

		omsg = msg;
		while(c = *msg++) putc(c, stdout);
		msg = omsg;
		if(pid){turn = 1; lseek(flagfd, 0,SEEK_SET); write(flagfd, &turn,1);}
		else{ turn = 0; lseek(flagfd, 0, SEEK_SET); write(flagfd,&turn, 1);}
	}
	return 0;
}
