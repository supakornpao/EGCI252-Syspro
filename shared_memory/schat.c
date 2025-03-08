#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MEM_SIZE 4096

struct shm_st{
  int written;
  char data[BUFSIZ];
};

int main(int argc, char *argv[]){

  int user = atoi(argv[1]);
  char buffer[BUFSIZ];
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  key_t key;
  key = ftok("/home/runner/", 21930);

  int shmid = shmget(key, MEM_SIZE, 0666|IPC_CREAT);
  if (shmid == -1) {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }
  sh_mem = shmat(shmid, NULL, 0);
  if (sh_mem == (void *)-1) {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }
  
  sh_area = (struct shm_st *)sh_mem;
  sh_area->written = 0;

  if(user == 1){
    
    int child;
    child = fork();
    switch(child){
      case -1: perror("Forking failed"); exit(EXIT_FAILURE);
      case 0: 
              while(strncmp(sh_area->data, "end chat", 8)){
                while(sh_area->written!=2){
                  sleep(1);
                }
                printf("%s\n",sh_area -> data);
                sh_area->written = 0;
                sleep(1);
              }
              break;
      default:while(strncmp(sh_area->data, "end chat", 8)){
                while (sh_area->written!=0) {
                  sleep(1);
                }
                printf("Enter data: ");
                fgets(buffer, BUFSIZ, stdin);
                strcpy(sh_area->data, buffer);
                sh_area->written = 1;
              }
              break;
    }
  }
  else if(user == 2){
      
      int child;
      child = fork();
      switch(child){
        case -1: perror("Forking failed"); exit(EXIT_FAILURE);
        case 0: 
                while(strncmp(sh_area->data, "end chat", 8)){
                  while(sh_area->written!=1){
                    sleep(1);
                  }
                  printf("%s\n",sh_area -> data);
                  sh_area->written = 0;
                  sleep(1);
                }
                break;
        default:while(strncmp(sh_area->data, "end chat", 8)){
                  while (sh_area->written!=0) {
                    sleep(1);
                  }
                  printf("Enter data: ");
                  fgets(buffer, BUFSIZ, stdin);
                  strcpy(sh_area->data, buffer);
                  sh_area->written = 2;
                }
                break;
      }
    }
}