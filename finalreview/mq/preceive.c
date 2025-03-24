#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<mqueue.h>
#include<string.h>

#define Q_NAME "/mq"
#define Q_PERM 0660
#define MAX_MSG_SIZE 256
#define MAX_MSG 3
#define BUF_SIZE 64

struct msg{
    int written;
    char data[BUF_SIZE];
};

int main(){
    int running = 1;
    unsigned int priority;
    struct msg amsg;
    priority = 0;
    struct mq_attr q_attr;
    q_attr.mq_maxmsg = MAX_MSG;
    q_attr.mq_msgsize = MAX_MSG_SIZE;
    mqd_t mqd;
    mqd = mq_open(Q_NAME, O_RDONLY|O_CREAT, Q_PERM, &q_attr);
    if(mqd == -1){
        perror("mq open error\n");
        exit(EXIT_FAILURE);
    }

    while(running){
        if(mq_receive(mqd, (void *)&amsg, MAX_MSG_SIZE, &priority)==-1){
            perror("mq receive fail\n");
            exit(EXIT_FAILURE);
        }
        printf("%s\n",amsg.data);
        if(strncmp(amsg.data,"end",3)==0)
            running = 0;
    }
    if(mq_unlink(Q_NAME)==-1){
        printf("unlink error\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}