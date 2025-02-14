#include<stdlib.h>
#include<stdio.h>

int main(){
	printf("Using system function for running ps program");
	system("ps -ax");
	printf("ps completed\n");
	return 0;
}
