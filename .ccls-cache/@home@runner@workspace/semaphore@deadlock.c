#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

static int set_semvalue(int);
static void del_semvalue(int);
static int semaphore_p(int);
static int semaphore_v(int);

static int s1,s2;

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};


void f2(pid_t);

void f1(pid_t pid){
  semaphore_p(s1);
  if(pid) f2(pid);
  semaphore_v(s1);
}

void f2(pid_t pid){
  semaphore_p(s2);
  if(!pid) f1(pid);
  semaphore_v(s2);
}

int main(int argc, char *argv[]){
  s1 = semget((key_t)1234, 1, 0666 | IPC_CREAT);
  s2 = semget((key_t)5678, 1, 0666 | IPC_CREAT);
  pid_t pid;
  pid = fork();

  if(pid == 0){
    f2(pid);
  }
  else if(pid>0){
    f1(pid);
  }
  printf("Done!\n");
  
}

static int set_semvalue(int s) {
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(s, 0, SETVAL, sem_union) == -1)
    return (0);
  return (1);
}

static void del_semvalue(int s) {
  union semun sem_union;

  if (semctl(s, 0, IPC_RMID, sem_union) == -1)
    fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(int s) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = -1; /* P() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(s, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_p failed\n");
    return (0);
  }
  return (1);
}

static int semaphore_v(int s) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = 1; /* V() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(s, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_v failed\n");
    return (0);
  }
  return (1);
}