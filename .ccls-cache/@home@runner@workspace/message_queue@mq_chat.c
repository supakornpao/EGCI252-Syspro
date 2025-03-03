#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mqueue.h>

struct msg{
  int msg_type;
  char msg_data[BUFSIZ+1];
};

int main(int argc, char *argv[]){
  int user = atoi(argv[1]);

  if(user==1){
    
    
  }
  else{
    
  }
  
}