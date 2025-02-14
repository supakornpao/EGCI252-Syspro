#include<stdio.h>

int main(int argc, char *argv[]){
	int status;
	if(argc<2){
		printf("\nCommand-line argument required!\n");
		exit(0);
	}
	if((status = system(argv[1]))<0){
		perror("system() error");
	}
	return 0;

}
