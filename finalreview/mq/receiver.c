#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct a_msg{
    long msg_type;
    char data[BUFSIZ];
} Message;

int main(){
    int msqid,running = 1;
    Message amsg;
    long int rcvmsg = 0;
    msqid = msgget((key_t)1234,0666|IPC_CREAT);
    if(msqid==-1){
        perror("msgget error\n");
        exit(EXIT_FAILURE);
    }
    while(running){
        if(msgrcv(msqid, (void*)&amsg, BUFSIZ,rcvmsg,0)==-1){
            perror("msgrcv error\n");
            exit(EXIT_FAILURE);
        }
        printf("%s\n",amsg.data);
        if(strncmp(amsg.data, "end",3)==0)
            running = 0;
    }

}