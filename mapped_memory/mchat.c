#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define FILE_LENGTH 0x100
#define FILE_NAME "/tmp/chat-log"

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  void *file_memory;
  int fd;

  char buffer[BUFSIZ];

  fd = open(FILE_NAME, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
      perror("File open failed");
      exit(EXIT_FAILURE);
  }
  if (ftruncate(fd, FILE_LENGTH) == -1) {
      perror("ftruncate failed");
      exit(EXIT_FAILURE);
  }
  file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (file_memory == MAP_FAILED) {
      perror("mmap failed");
      exit(EXIT_FAILURE);
  }
  close(fd);

  if(user == 1){
    pid_t pid;
    pid = fork();
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(buffer, "end chat", 8)){
                if (strlen((char *)file_memory) > 0) {
                  sscanf(file_memory, "%255s", buffer);
                }
                printf("%s\n", buffer);
                sleep(1);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
      default:while(strncmp(buffer, "end chat", 8)){
                fgets(buffer, BUFSIZ, stdin);
                lseek(fd, FILE_LENGTH + 1, SEEK_SET); 
                write(fd, "", 1);
                //lseek(fd, 0, SEEK_SET);
                snprintf((char *)file_memory, FILE_LENGTH, "%s", buffer);
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
                if (strlen((char *)file_memory) > 0) {
                  sscanf((char *)file_memory, "%255s", buffer);
                }
                printf("%s\n", buffer);
                sleep(1);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
      default:while(strncmp(buffer, "end chat", 8)){
                fgets(buffer, BUFSIZ, stdin);
                lseek(fd, FILE_LENGTH + 1, SEEK_SET); 
                write(fd, "", 1);
                //lseek(fd, 0, SEEK_SET);
                snprintf((char *)file_memory, FILE_LENGTH, "%s", buffer);
              }
              munmap(file_memory, FILE_LENGTH);
              break;
    }
  }
  
}