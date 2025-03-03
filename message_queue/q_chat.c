#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mqueue.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msg{
  int msg_type;
  char msg_data[BUFSIZ+1];
};

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  int student = atoi(argv[2]);

  
  int msgID = msgget((key_t) student,0666|IPC_CREAT);
  
  
  if(user==1){
    struct msg amsg;
    pid_t pid;
    pid = fork();
    
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: 
      default: 
    }
    
  }
  else{

    pid_t pid;
    pid = fork();

    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: 
      default: 
    }
    
  }
  
}