#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(){
  FILE* read_fp;
  int char_read;
  char buffer[BUFSIZ+1];

  memset(buffer, '\0', sizeof(buffer));
  read_fp = popen("uname -a", "r");

  if(read_fp != NULL){
    char_read = fread(buffer, sizeof(char), sizeof(buffer), read_fp);
    if(char_read>0){
      printf("Output from pipe: %s\n", buffer);
    }
    pclose(read_fp);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}