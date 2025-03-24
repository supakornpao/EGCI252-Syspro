#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<string.h>
#include<unistd.h>

#define SHM_NAME "/psh"
#define MEM_SIZE 4096

struct shm_st{
    int written;
    char data[BUFSIZ];
};

int main(){
    int running = 1, st_size, shmfd;
    char buffer[BUFSIZ];
    struct shm_st* sh_area;
    void* sh_mem = NULL;

    shmfd = shm_open(SHM_NAME,O_CREAT|O_RDWR,0660);
    if(shmfd == -1){
        perror("shmfd error\n");
        exit(EXIT_FAILURE);
    }
    st_size = sizeof(struct shm_st);
    ftruncate(shmfd, st_size);
    sh_mem = mmap(0,st_size, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);

    sh_area = (struct shm_st*)sh_mem;

    while(running){
        while(sh_area->written==1)
            sleep(1);
        fgets(buffer, BUFSIZ, stdin);
        strcpy(sh_area->data,buffer);
        sh_area->written = 1;
        if(strncmp(sh_area->data,"end",3)==0)
            running = 0;
    }
    munmap(sh_mem,st_size);
    close(shmfd);
    exit(EXIT_SUCCESS);
}