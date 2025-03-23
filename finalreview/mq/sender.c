#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct a_msg{
    long int msg_type;
    char data[BUFSIZ];
} Message;

int main(){
    int msqid, running = 1;
    char buffer[BUFSIZ];
    Message amsg;
    msqid = msgget((key_t)1234,0666|IPC_CREAT);
    if(msqid==-1){
        perror("msgget error\n");
        exit(EXIT_FAILURE);
    }
    while(running){
        fgets(buffer,sizeof(buffer),stdin);
        amsg.msg_type = 1;
        strcpy(amsg.data,buffer);
        msgsnd(msqid,(void*)&amsg,sizeof(buffer),amsg.msg_type);
        if(strncmp(buffer,"end",3)==0) running=0;
    }
}