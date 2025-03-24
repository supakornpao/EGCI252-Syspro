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
    int running = 1, shmfd, st_size;
    void *sh_mem = NULL;
    struct shm_st* sh_area;
    char buffer[BUFSIZ];

    st_size = sizeof(struct shm_st);
    shmfd = shm_open(SHM_NAME,O_CREAT|O_RDWR,0660);
    if(shmfd==-1){
        perror("shm open error\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(shmfd, st_size);
    sh_mem = mmap(0,st_size,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
    if(sh_mem ==(void*) -1){
        perror("sh mem error\n");
        exit(EXIT_FAILURE);
    }
    sh_area = (struct shm_st*)sh_mem;
    sh_area->written = 0;

    while(running){
        while(sh_area->written==0)
            sleep(1);
        printf("%s\n",sh_area->data);
        sh_area->written = 0;
        if(strncmp(sh_area->data,"end",3)==0)
            running = 0;
    }
    munmap(sh_mem, st_size);
    close(shmfd);
    if(shm_unlink(SHM_NAME)==-1){
        perror("unlink Failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}