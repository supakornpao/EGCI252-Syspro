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

struct file_memory {
  volatile int written;
  char text[FILE_LENGTH-sizeof(int)];
};

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  int fd;

  char buffer[BUFSIZ];

  fd = open(FILE_NAME, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
      perror("File open failed");
      exit(EXIT_FAILURE);
  }
  if(read(fd,buffer,sizeof(buffer)>0)){
    system("echo "" > /tmp/chat-log"); //clear previous chat log
  }
  if (ftruncate(fd, FILE_LENGTH) == -1) {
      perror("ftruncate failed");
      exit(EXIT_FAILURE);
  }
  struct file_memory * file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (file_memory == MAP_FAILED) {
      perror("mmap failed");
      exit(EXIT_FAILURE);
  }
  file_memory->written = 0;

  if(user == 1){
    pid_t pid;
    pid = fork();
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(buffer, "end chat", 8)){
                while(file_memory->written!=2)
                    sleep(1);
                strncpy(buffer, file_memory->text, sizeof(buffer) - 1);
                buffer[sizeof(buffer)-1] = '\0';
                printf(">> %s", buffer);
                file_memory->written = 0;
                sleep(1);
              }
              
              munmap(file_memory, FILE_LENGTH);
              exit(0);
              
      default:while(strncmp(buffer, "end chat", 8)){
                while(file_memory->written!=0)
                    sleep(1);
                fgets(buffer, BUFSIZ, stdin);
                strncpy(file_memory->text, buffer, sizeof(file_memory->text) - 1);
                file_memory->text[sizeof(file_memory->text) - 1] = '\0';
                file_memory->written = 1;
              }
              wait(NULL);
              munmap(file_memory, FILE_LENGTH);
              close(fd);
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
                
                while(file_memory->written!=1)
                  sleep(1);
                strncpy(buffer, file_memory->text, sizeof(buffer) - 1);
                buffer[sizeof(buffer)-1] = '\0';
                printf(">> %s", buffer);
                file_memory->written = 0;
                sleep(1);
              }
              munmap(file_memory, FILE_LENGTH);
              exit(0);
              
      default:while(strncmp(buffer, "end chat", 8)){
                while(file_memory->written!=0)
                  sleep(1);
                fgets(buffer, BUFSIZ, stdin);
                strncpy(file_memory->text, buffer, sizeof(file_memory->text) - 1);
                file_memory->text[sizeof(file_memory->text) - 1] = '\0';
                file_memory->written = 2;
              }
              wait(NULL);
              munmap(file_memory, FILE_LENGTH);
              close(fd);
              break;
    }
  }
  
}