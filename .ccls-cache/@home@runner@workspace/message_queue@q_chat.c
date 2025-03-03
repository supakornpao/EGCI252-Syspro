#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mqueue.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct msg{
  long int msg_type;
  char msg_data[BUFSIZ];
};

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  int student = atoi(argv[2]);
  char buffer[BUFSIZ];
  
  int msgID = msgget((key_t) student,0666|IPC_CREAT);
  
  
  if(user==1){
    long int rcvtype = 2;
    pid_t pid;
    pid = fork();
    struct msg amsg;
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: 
        while(strncmp(amsg.msg_data,"end chat",8)){
          if(msgrcv(msgID,(void*)&amsg,BUFSIZ,rcvtype,0)==-1){
            fprintf(stderr,"msgrcv failed\n");
            exit(EXIT_FAILURE);
          }
          printf("Received Message: %s\n",amsg.msg_data);
        }
        
      default: 
        while(strncmp(amsg.msg_data,"end chat",8)){
          printf("Enter data: ");
          fgets(buffer, BUFSIZ, stdin);
          amsg.msg_type = 1;
          strcpy(amsg.msg_data, buffer);
          if(msgsnd(msgID, (void*) &amsg, BUFSIZ, 0)==-1){
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
          }
        }
    }
    
  }
  else{
    long int rcvtype = 1;
    pid_t pid;
    pid = fork();
    struct msg amsg;
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: 
        while(strncmp(amsg.msg_data,"end chat",8)){
          if(msgrcv(msgID,(void*)&amsg,BUFSIZ,rcvtype,0)==-1){
            fprintf(stderr,"msgrcv failed\n");
            exit(EXIT_FAILURE);
          }
          printf("Received Message: %s\n",amsg.msg_data);
        }
      default:
        while (strncmp(amsg.msg_data, "end chat", 8)) {
          printf("Enter data: ");
          fgets(buffer, BUFSIZ, stdin);
          amsg.msg_type = 2;
          strcpy(amsg.msg_data, buffer);
          if (msgsnd(msgID, (void *)&amsg, BUFSIZ, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
          }
        }
    }
    
  }
  
}