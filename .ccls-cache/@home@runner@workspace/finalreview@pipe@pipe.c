#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(){
  FILE* read_fp;
  char buffer[BUFSIZ+1];

  read_fp = fopen("uname -a","r");
}