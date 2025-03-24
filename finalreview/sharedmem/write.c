#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define MEM_SIZE 4096

typedef struct shm_st{
    int written;
    char data[BUFSIZ];
} Sharedmemory;

int main(){
    int running=1, shmid;
    void *sh_mem = NULL;
    Sharedmemory* sh_area;
    char buffer[BUFSIZ];
    shmid = shmget((key_t)1234, MEM_SIZE, 0666|IPC_CREAT);
    if(shmid == -1){
        perror("shmid error\n");
        exit(EXIT_FAILURE);
    }

    sh_mem = shmat(shmid, NULL, 0);
    if(sh_mem == (void*) -1){
        perror("sh_mem error\n");
        exit(EXIT_FAILURE);
    }
    sh_area = (Sharedmemory*)sh_mem;
    //sh_area->written = 0;
    while(running){
        while(sh_area->written!=0)
            sleep(1);

        printf("Enter Data: ");
        fgets(buffer,BUFSIZ,stdin);
        strcpy(sh_area->data, buffer);
        sh_area->written = 1;
        if(strncmp(sh_area->data,"end",3)==0)
            running = 0;
    }
    if(shmdt(sh_mem)==-1){
        perror("Detach error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}