#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<unistd.h>
#include <sys/mman.h>
#include<wait.h>


#define SHM_NAME "/pshm"
#define MEM_SIZE 64

struct shm_st{
    sem_t sem;
    char data[MEM_SIZE];
};

int main(){
    void* sh_mem = NULL;
    struct shm_st* sh_area;
    int shmid;
    pid_t pid;
    char C;
    shmid = shm_open(SHM_NAME,O_RDWR|O_CREAT,0666);
    if(shmid==-1){
        perror("shm open failed\n");
        exit(EXIT_FAILURE);
    }

    int st_area = sizeof(struct shm_st); 
    int i = ftruncate(shmid, st_area);
    sh_mem = mmap(0, st_area, PROT_READ|PROT_WRITE,MAP_SHARED,shmid,0);
    sh_area = (struct shm_st*) sh_mem;
    
    if(sem_init(&sh_area->sem,1,1)){
        perror("sem init error\n");
        exit(EXIT_FAILURE);
    }

    for(i=0;i<10;i++){

    }

    pid = fork();
    if(pid) C='O';
    else C = 'X';
    
    for(i=0;i<10;i++){
        if(sem_wait(&sh_area->sem))
            exit(EXIT_FAILURE);
        printf("%c",C);
        fflush(stdout);
        usleep(1000);
        printf("%c",C);
        fflush(stdout);
        if(sem_post(&sh_area->sem))
            exit(EXIT_FAILURE);
        usleep(1000);
    }
    if(pid){
        usleep(1000);
        sem_destroy(&sh_area->sem);
        wait(NULL);
        printf("\n %d Finished", getpid());
    }

    else printf("\n %d Finished", getpid());
    exit(EXIT_FAILURE);
}