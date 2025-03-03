#include <time.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define Q_NAME "/my_mq"
#define Q_PERM 0660
#define MAX_MSG 3
#define MAX_MSG_SIZE 256
#define BUF_SIZE 64

struct msgs{
  int written;
  char data[BUF_SIZE];
};

int main(void){
  int running = 1;
  struct msgs a_msg; 
  struct mq_attr q_attr;

  mqd_t mqd;
  unsigned int priority = 0;

  char buffer[BUF_SIZE], *s;
  struct timespec to;

  mqd = mq_open(Q_NAME, O_WRONLY | O_CREAT, Q_PERM, &q_attr);

  if(mqd == -1){
    perror("mq_open failed ");
    exit(EXIT_FAILURE);
  }

  while(running){
    printf("Enter data: ");
    s = fgets(buffer, BUF_SIZE, stdin);
    printf( "Size of msgs: %lu\n", sizeof(struct msgs) );
    strcpy(a_msg.data, buffer);
    clock_gettime(CLOCK_REALTIME, &to);
    to.tv_sec+=5;
    if(mq_timedsend(mqd, (void *)&a_msg, sizeof(struct msgs), priority,&to)==-1){
      perror("mq_send failed ");
      exit(EXIT_FAILURE);
    }
    if(strncmp(buffer, "end", 3) == 0){
      running = 0;
    }
  }
  mq_close(mqd);
  exit(EXIT_SUCCESS);
  
}