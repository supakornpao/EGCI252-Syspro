#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(){
  pid_t pid;

  int pipefd[2], n_bytes;
  char buffer[BUFSIZ];
  char *message = "Hello world";
  memset(buffer, '\0', BUFSIZ);

  if(pipe(pipefd)==0){
    pid = fork();

    switch(pid){
      case -1: perror("fork failed"); exit(EXIT_FAILURE);
      case 0:  sprintf(buffer, "%d", pipefd[0]);
               execl("reader", "reader", buffer, (char*) 0);
               break;
      default: n_bytes = write(pipefd[1], message, strlen(message));
               printf("Sent %d bytes\n", n_bytes);
               break;
    }
  }
  
  
}