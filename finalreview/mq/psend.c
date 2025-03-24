#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<mqueue.h>

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
    int running=1;
    unsigned int priority;
    struct msg amsg;
    char buffer[BUF_SIZE];
    mqd_t mqd;

    struct mq_attr q_attr;
    
    priority = 0;

    q_attr.mq_msgsize = MAX_MSG_SIZE;
    q_attr.mq_maxmsg = MAX_MSG;

    mqd = mq_open(Q_NAME, O_WRONLY|O_CREAT,Q_PERM,&q_attr);
    if(mqd == -1){
        perror("mq_open error!\n");
        exit(EXIT_FAILURE);
    }

    while(running){
        printf("Enter data: ");
        fgets(buffer, BUF_SIZE, stdin);
        strcpy(amsg.data,buffer);
        if(mq_send(mqd,(void*)&amsg,sizeof(struct msg),priority) == -1){
            perror("msg send failed\n");
            exit(EXIT_FAILURE);
        }
        if(strncmp(amsg.data,"end",3)==0)
            running = 0;
    }
    exit(EXIT_SUCCESS);
}