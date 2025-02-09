#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	int pid = fork();

	if(pid != 0)
		fork();
	printf("Hello world\n");
	return 0;
}
