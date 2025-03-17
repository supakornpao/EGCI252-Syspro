#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};


void f2(pid_t);

void f1(pid_t pid){
  semaphore_p();
  if(pid) f2(pid);
  printf("Done\n");
  semaphore_v();
}

void f2(pid_t pid){
  semaphore_p();
  if(!pid) f1(pid);
  printf("Done\n");
  semaphore_v();
}

int main(int argc, char *argv[]){
  sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
  pid_t pid;
  pid = fork();

  if(pid == 0){
    f2(pid);
  }
  else if(pid>0){
    f1(pid);
  }
}

static int set_semvalue(void) {
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    return (0);
  return (1);
}

static void del_semvalue(void) {
  union semun sem_union;

  if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(void) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = -1; /* P() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(sem_id, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_p failed\n");
    return (0);
  }
  return (1);
}

static int semaphore_v(void) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = 1; /* V() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(sem_id, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_v failed\n");
    return (0);
  }
  return (1);
}