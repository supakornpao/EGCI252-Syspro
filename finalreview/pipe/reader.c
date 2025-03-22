#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
  int fd, n_bytes;

  char buffer[BUFSIZ+1];
  memset(buffer, '\0', sizeof(buffer));
  sscanf(argv[1],"%d",&fd);

 
  n_bytes = read(fd, buffer, sizeof(buffer));
  printf("Read %d characters from pipe\n", n_bytes);
  exit(EXIT_SUCCESS);
  
}