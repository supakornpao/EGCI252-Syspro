#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mqueue.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#include <wait.h>

struct msg{
  long int msg_type;
  char msg_data[BUFSIZ];
};

pid_t other_user_pid = -1;

void sighandler(int signal){
  if(signal == SIGUSR1) printf("User 1 terminated\n");
  else if(signal == SIGUSR2) printf("User 2 terminated\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  int studentid = 6581117;
  int mypid = getpid();
  char buffer[BUFSIZ];
  struct msg amsg, pidmsg, pidrcv;
  
  int msgID = msgget((key_t) studentid,0666|IPC_CREAT);


  if(user==1){
    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);
    long int rcvtype = 2;
    pidmsg.msg_type = 3;
    sprintf(pidmsg.msg_data, "%d", mypid);
    msgsnd(msgID, (void*)&pidmsg, sizeof(pidmsg.msg_data), 0);
    msgrcv(msgID,(void*)&pidrcv,sizeof(pidrcv.msg_data),3,0);
    other_user_pid = atoi(pidrcv.msg_data);
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
          printf(">> %s",amsg.msg_data);
        }
        kill(getppid(), SIGUSR1);  
        exit(EXIT_SUCCESS);

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
        if (other_user_pid != -1) {
          kill(other_user_pid, SIGUSR1);
        }
        kill(pid, SIGUSR2);
        exit(EXIT_SUCCESS);
        break;
    }

  }
  else if(user==2){
    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);
    long int rcvtype = 1;
    pidmsg.msg_type = 3;
    sprintf(pidmsg.msg_data, "%d", mypid);
    msgsnd(msgID, (void*)&pidmsg, sizeof(pidmsg.msg_data), 0);
    msgrcv(msgID,(void*)&pidrcv,sizeof(pidrcv.msg_data),pidmsg.msg_type,0);
    other_user_pid = atoi(pidrcv.msg_data);
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
          printf(">> %s",amsg.msg_data);
        }
        kill(getppid(), SIGUSR2);  
        exit(EXIT_SUCCESS);
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
        if (other_user_pid != -1) {
          kill(other_user_pid, SIGUSR2);
        }
        kill(pid, SIGUSR1);
        exit(EXIT_SUCCESS);
        break;
    }

  }

}