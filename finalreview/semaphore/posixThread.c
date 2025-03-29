#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<pthread.h>

#define sem1 "/sem1"
#define sem2 "/sem2"

struct args{
    sem_t *sem;
    int count;
    char *C;
};

void* func(void* parameter){
    struct args* p = (struct args*)parameter;
    sem_wait(p->sem);
    for(int i=0;i<p->count;i++){
        printf("%s",p->C);
    }
    sem_post(p->sem);
    printf("\n");
    
    return NULL;
}

int main(){
    sem_t * s1, *s2;
    pthread_t threads[2];
    struct args argsth[2];

    s1 = sem_open(sem1,O_RDWR|O_CREAT,0660,1);
    s2 = sem_open(sem2,O_RDWR|O_CREAT,0660,1);

    argsth[0].sem = s1;
    argsth[0].count = 10;
    argsth[0].C = "OO";
    argsth[1].sem = s2;
    argsth[1].count = 20;
    argsth[1].C = "YY";

    for(int i=0;i<2;i++){
        pthread_create(&threads[i],NULL,&func,&argsth[i]);
    }

    for(int i=0;i<2;i++){
        pthread_join(threads[i],NULL);
    }
    sem_unlink(sem1);
    sem_unlink(sem2);
}