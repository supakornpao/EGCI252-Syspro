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
  int studentid = atoi(argv[2]);
  char buffer[BUFSIZ];
  struct msg amsg;
  int msgID = msgget((key_t) studentid,0666|IPC_CREAT);
  
  
  if(user==1){
    long int rcvtype = 2;
    pid_t pid;
    pid = fork();
    
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
          if(strncmp(amsg.msg_data,"end chat",8)!=0)printf(">> %s",amsg.msg_data);
        }
        break;
        
      default: 
        while(strncmp(amsg.msg_data,"end chat",8)){
          
          fgets(buffer, BUFSIZ, stdin);
          amsg.msg_type = 1;
          strcpy(amsg.msg_data, buffer);
          if(msgsnd(msgID, (void*) &amsg, BUFSIZ, 0)==-1){
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
          }
        }
        break;
    }
    
  }
  else if(user==2){
    long int rcvtype = 1;
    pid_t pid;
    pid = fork();
    
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
          if(strncmp(amsg.msg_data,"end chat",8)!=0) printf(">> %s",amsg.msg_data);
        }
        break;
      default:
        while (strncmp(amsg.msg_data, "end chat", 8)) {
          fgets(buffer, BUFSIZ, stdin);
          amsg.msg_type = 2;
          strcpy(amsg.msg_data, buffer);
          if (msgsnd(msgID, (void *)&amsg, BUFSIZ, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
          }
        }
        break;
    }
    
  }
  
}