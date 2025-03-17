#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define SHM_NAME "/pshm"
#define MEM_SIZE 64
struct shm_st {
  atomic_int written;
  char data[MEM_SIZE];
};

void *sh_mem = NULL;
int st_size = sizeof(struct shm_st);

void sighandler(int signal) {
  if(signal==SIGTERM){
    munmap(sh_mem, st_size);
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char* argv[]){
  
  int shmfd;
  int user = atoi(argv[1]);
  struct shm_st *sh_area;

  char buffer[BUFSIZ];

  shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);
  if (shmfd == -1) {
    perror("shm_open failed ");
    exit(EXIT_FAILURE);
  }
  ftruncate(shmfd, st_size);
  sh_mem = mmap(0, st_size, PROT_READ | PROT_WRITE, MAP_SHARED,shmfd, 0);
  sh_area = (struct shm_st *)sh_mem;

  atomic_store(&sh_area->written, 0);

  if(user == 1){
    pid_t pid;
    pid = fork();
    signal(SIGTERM,sighandler);
    switch(pid){
      case -1: 
      case 0: while(strncmp(sh_area->data, "end chat", 8)){
                if(atomic_load(&sh_area->written)==2){
                  printf(">> %s", sh_area->data);
                  atomic_store(&sh_area->written, 0);
                }
              }
              //munmap(sh_mem, st_size);
              close(shmfd);
              if (shm_unlink(SHM_NAME) == -1) {
                perror("shm_unlink failed ");
                exit(EXIT_FAILURE);
              }
              kill(getppid(),SIGTERM);
              raise(SIGTERM);
              break;
      default:while(strncmp(sh_area->data, "end chat", 8)){
                while (atomic_load(&sh_area->written))
                  usleep(1000);
                fgets(buffer, BUFSIZ, stdin);
                strcpy(sh_area->data, buffer);
                atomic_store(&sh_area->written, 1);
              }
              kill(pid,SIGTERM);
              raise(SIGTERM);
              break;
    }
  }
  else if(user == 2){
    pid_t pid;
    pid = fork();
    signal(SIGTERM,sighandler);
    switch(pid){
      case -1: 
      case 0: while(strncmp(sh_area->data, "end chat", 8)){
                if (atomic_load(&sh_area->written) == 1) {
                  printf(">> %s", sh_area->data);
                  atomic_store(&sh_area->written, 0);
                }
              }
              //munmap(sh_mem, st_size);
              close(shmfd);
              kill(getppid(), SIGTERM);
              raise(SIGTERM);
              break;
      default:while(strncmp(sh_area->data, "end chat", 8)){
                while (atomic_load(&sh_area->written))
                  usleep(1000);
                fgets(buffer, BUFSIZ, stdin);
                strcpy(sh_area->data, buffer);
                atomic_store(&sh_area->written, 2);
              }
              kill(pid, SIGTERM);
              raise(SIGTERM);
              break;
    }
  }
}