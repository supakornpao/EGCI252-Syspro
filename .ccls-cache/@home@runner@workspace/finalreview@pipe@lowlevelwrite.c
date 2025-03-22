#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(){
  int pipefd[2], n_bytes;
  char buffer[BUFSIZ];
  char *str = "Hello World";

  memset(buffer, '\0', sizeof(buffer));
  
  if(pipe(pipefd)==0){
    n_bytes = write(pipefd[1], str, strlen(str));
    n_bytes = read(pipefd[0],buffer,BUFSIZ);
    printf("Size of string: %d\n", n_bytes);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}