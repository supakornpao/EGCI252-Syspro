#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    char C;
    sem_t *s;
    s = sem_open("/sem",O_CREAT|O_RDWR,0660,1);
    if(argc>1){
        C = 'X';
    }
    else C = 'O';
    for(int i=0;i<10;i++){
        if(sem_wait(s)==-1){
            exit(EXIT_FAILURE);
        }
        printf("%c",C);
        fflush(stdout);
        usleep(1000);
        printf("%c",C);
        fflush(stdout);
        if(sem_post(s)==-1){
            exit(EXIT_FAILURE);
        }
        usleep(1000);
    }
    printf("\n %d Finished",getpid());
    if(argc>1){
        sem_unlink("/sem");
    }
    exit(EXIT_SUCCESS);
}