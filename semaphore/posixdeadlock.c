#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <sys/wait.h>
#define SEM_NAME1 "/sem1"
#define SEM_NAME2 "/sem2"

sem_t *s1, *s2;

void f2(pid_t);
void f1(pid_t pid){
  if(sem_wait(s1)==-1){
    perror("sem_wait failed");
    //exit(EXIT_FAILURE);
  }
    
  if(pid!=0) f2(pid);
  if(sem_post(s1) == -1){
    perror("sem_wait failed");
    //exit(EXIT_FAILURE);
  }
}

void f2(pid_t pid){
  if(sem_wait(s2)==-1){
    perror("sem_wait failed");
    //exit(EXIT_FAILURE);
  }
  if(pid==0) f1(pid);
  if(sem_post(s2) == -1){
    perror("sem_wait failed");
    //exit(EXIT_FAILURE);
  }
}

int main(){
  
  s1 = sem_open(SEM_NAME1, O_CREAT| O_RDWR,0660,0); 
  if(s1 == SEM_FAILED)
    perror("sem_open failed");
  s2 = sem_open(SEM_NAME2, O_CREAT| O_RDWR,0660,0); 
  if(s2 == SEM_FAILED)
    perror("sem_open failed");
  pid_t pid;
  pid = fork();
  if(pid==0){
    f1(pid);
  }
  else{
    f2(pid);
  }
  sem_unlink(SEM_NAME1);
  sem_unlink(SEM_NAME2);
  printf("Done!\n");
  //exit(EXIT_SUCCESS);
}