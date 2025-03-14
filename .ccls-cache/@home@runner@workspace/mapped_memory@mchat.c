#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define FILE_LENGTH 0x100
#define FILE_NAME "/tmp/chat-log"

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  void *file_memory;
  int fd;

  char buffer[BUFSIZ];

  fd = open(FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
  file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  if(user == 1){
    pid_t pid;
    pid = fork();
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(buffer, "end chat", 8)){
                sscanf(file_memory, "%s", buffer);
                printf("%s\n", buffer);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
      default:while(strncmp(buffer, "end chat", 8)){
                fgets(buffer, sizeof(buffer), stdin);
                lseek(fd, FILE_LENGTH + 1, SEEK_SET); 
                write(fd, "", 1);
                lseek(fd, 0, SEEK_SET);
                sprintf((char *)file_memory, "%s", buffer);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
    }
  }
  else if(user == 2){
    pid_t pid;
    pid = fork();

    switch(pid){
      case -1: 
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(buffer, "end chat", 8)){
                sscanf(file_memory, "%s", buffer);
                printf("%s\n", buffer);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
      default:while(strncmp(buffer, "end chat", 8)){
                fgets(buffer, sizeof(buffer), stdin);
                lseek(fd, FILE_LENGTH + 1, SEEK_SET); 
                write(fd, "", 1);
                lseek(fd, 0, SEEK_SET);
                sprintf((char *)file_memory, "%s", buffer);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
    }
  }
  
}