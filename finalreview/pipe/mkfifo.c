#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define FIFO "/tmp/fifo"

int main(int argc, char* argv[]){
    int res, open_mode = 0;
    if(argc<2){
        printf("Usage: %s <[O_RDONLY,O_WRONLY,O_RDWR,O_NONBLOCK]>",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if(argv[1]){
        if(strncmp(argv[1],"O_RDONLY",8)==0)
            open_mode|=O_RDONLY;
        if(strncmp(argv[1],"O_WRONLY",8)==0)
            open_mode|=O_WRONLY;
        if(strncmp(argv[1],"O_RDWR",6)==0)
            open_mode|=O_RDWR;
        if(strncmp(argv[1],"O_NONBLOCK",10)==0)
            open_mode|=O_RDONLY|O_NONBLOCK;
    }
    //printf("open_mode: %d\n",open_mode);
    if(access(FIFO,F_OK)==-1){
        res = mkfifo(FIFO,0777);
        if(res==-1){
            printf("Open fifo failed\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Process %d is opening FIFO\n",getpid());
    res = open(FIFO,open_mode);
    printf("Process %d result = %d\n",getpid(),res);
    sleep(5);

    printf("Process %d finished\n",getpid());
    exit(EXIT_SUCCESS);
}