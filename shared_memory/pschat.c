#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_NAME "/pshm"
#define MEM_SIZE 64
struct shm_st {
  atomic_int written;
  char data[MEM_SIZE];
};

int main(int argc, char* argv[]){
  
  int shmfd, st_size;
  int user = atoi(argv[1]);
  struct shm_st *sh_area;
  void *sh_mem = NULL;
  char buffer[BUFSIZ];
  st_size = sizeof(struct shm_st);
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
    switch(pid){
      case -1: 
      case 0: while(strncmp(sh_area->data, "end chat", 8)){
                if(atomic_load(&sh_area->written)==2){
                  printf(">> %s", sh_area->data);
                  atomic_store(&sh_area->written, 0);
                }
              }
              munmap(sh_mem, st_size);
              close(shmfd);
              if (shm_unlink(SHM_NAME) == -1) {
                perror("shm_unlink failed ");
                exit(EXIT_FAILURE);
              }
              break;
      default:while(strncmp(sh_area->data, "end chat", 8)){
                while (atomic_load(&sh_area->written))
                  usleep(1000);
                fgets(buffer, BUFSIZ, stdin);
                strcpy(sh_area->data, buffer);
                atomic_store(&sh_area->written, 1);
              }
              break;
    }
  }
  else if(user == 2){
    pid_t pid;
    pid = fork();
    switch(pid){
      case -1: 
      case 0: while(strncmp(sh_area->data, "end chat", 8)){
                if (atomic_load(&sh_area->written) == 1) {
                  printf(">> %s", sh_area->data);
                  atomic_store(&sh_area->written, 0);
                }
              }
              munmap(sh_mem, st_size);
              close(shmfd);
              break;
      default:while(strncmp(sh_area->data, "end chat", 8)){
                while (atomic_load(&sh_area->written))
                  usleep(1000);
                fgets(buffer, BUFSIZ, stdin);
                strcpy(sh_area->data, buffer);
                atomic_store(&sh_area->written, 2);
              }
              break;
    }
  }
}