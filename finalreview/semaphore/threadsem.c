#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#define SEM_KEY_1 1234
#define SEM_KEY_2 5678

pthread_t O[10], X[20];

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  };

static int set_semvalue(int sem);
static void del_semvalue(int sem);
static int semaphore_p(int sem);
static int semaphore_v(int sem);

int semid1, semid2;

void* funcO(void* parameter){
    semaphore_p(semid1);
    printf("O");
    fflush(stdout);
    int time = rand()%3+1;
    sleep(time);
    semaphore_v(semid1);
    return NULL;
}

void* funcX(void* parameter){
    semaphore_p(semid2);
    printf("X");
    fflush(stdout);
    int time = rand()%3+1;
    sleep(time);
    semaphore_v(semid2);
    return NULL;
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    semid1 = semget((key_t)SEM_KEY_1,1,IPC_CREAT|0666);
    semid2 = semget((key_t)SEM_KEY_2,1,IPC_CREAT|0666);

    if(semid1 == -1 || semid2 == -1){
        perror("semaphore opening error");
        exit(EXIT_FAILURE);
    }

    if(!set_semvalue(semid1) || !set_semvalue(semid2)){
        printf("set_semvalue error");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<10;i++){
        pthread_create(&O[i],NULL,&funcO,NULL);
    }
    for(int i=0;i<20;i++){
        pthread_create(&X[i],NULL,&funcX,NULL);
    }

    for(int i=0;i<10;i++){
        pthread_join(O[i],NULL);
    }

    for(int i=0;i<20;i++){
        pthread_join(X[i],NULL);
    }

    del_semvalue(semid1);
    del_semvalue(semid2);
    exit(EXIT_SUCCESS);

}


static int set_semvalue(int sem_id) {
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
      return (0);
    return (1);
  }
  
  static void del_semvalue(int sem_id) {
    union semun sem_union;
  
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
      fprintf(stderr, "Failed to delete semaphore\n");
  }
  
  static int semaphore_p(int sem_id) {
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
  
  static int semaphore_v(int sem_id) {
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