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

void* file_memory;

struct file_memory {
  volatile int written;
  char text[FILE_LENGTH-sizeof(int)];
};



void sighandler(int signal){
  if(signal == SIGTERM){
    munmap(file_memory, FILE_LENGTH);
    system("rm -f chat_log");
    //printf("User terminated\n");
    exit(EXIT_SUCCESS);
  }
  
}

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);
  int fd, mypid = getpid();
  char buffer[BUFSIZ];

  fd = open(FILE_NAME, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
      perror("File open failed");
      exit(EXIT_FAILURE);
  }
  if(read(fd,buffer,sizeof(buffer)>0)){
    system("echo "" > /tmp/chat-log"); 
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
  struct file_memory* filearea = (struct file_memory*)file_memory;
  filearea->written = 0;

  if(user == 1){
    pid_t pid;
    pid = fork();
    signal(SIGTERM, sighandler);
    switch(pid){
      case -1:
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(filearea->text, "end chat", 8)){
                while(filearea->written!=2)
                    sleep(1);
                strncpy(buffer, filearea->text, sizeof(buffer) - 1);
                buffer[sizeof(buffer)-1] = '\0';
                printf(">> %s", buffer);
                filearea->written = 0;
                sleep(1);
              }
              kill(getppid(), SIGTERM);
              raise(SIGTERM);
              break;

      default:while(strncmp(filearea->text, "end chat", 8)){
                while(filearea->written!=0)
                    sleep(1);
                fgets(buffer, BUFSIZ, stdin);
                strncpy(filearea->text, buffer, sizeof(filearea->text) - 1);
                filearea->text[sizeof(filearea->text) - 1] = '\0';
                filearea->written = 1;
              }
              kill(pid, SIGTERM);
              raise(SIGTERM);
              break;
    }
  }
  else if(user == 2){
    
    pid_t pid;
    pid = fork();
    signal(SIGTERM, sighandler);

    switch(pid){
      case -1: 
        perror("Forking failed");
        exit(EXIT_FAILURE);
      case 0: while(strncmp(filearea->text, "end chat", 8)){
                while(filearea->written!=1)
                  sleep(1);
                strncpy(buffer, filearea->text, sizeof(buffer) - 1);
                buffer[sizeof(buffer)-1] = '\0';
                printf(">> %s", buffer);
                filearea->written = 0;
                sleep(1);
              }
              kill(getppid(), SIGTERM);
              raise(SIGTERM);
              break;

      default:while(strncmp(filearea->text, "end chat", 8)){
                while(filearea->written!=0)
                  sleep(1);
                fgets(buffer, BUFSIZ, stdin);
                strncpy(filearea->text, buffer, sizeof(filearea->text) - 1);
                filearea->text[sizeof(filearea->text) - 1] = '\0';
                filearea->written = 2;
              }
              kill(pid, SIGTERM);
              raise(SIGTERM);
              break;
    }
  }
  close(fd);
  

}