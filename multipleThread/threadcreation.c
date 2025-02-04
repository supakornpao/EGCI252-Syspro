#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

void *print_Xs(void *unused){
	int n;
	for(n=100;n>0;n--){
		fputc('X',stdout); usleep(1);
	}
	return NULL;
}

void *print_Ys(void* unused){
	int n;
	for(n=100;n>0;n--){
		fputc('Y',stdout); usleep(1);
	}
	return NULL;
}

int main(){
	int n;
	pthread_t thread_id1, thread_id2;

	pthread_create(&thread_id1,NULL,&print_Xs,NULL);
	//create thread that runs on print_Xs
	pthread_create(&thread_id2,NULL,&print_Ys,NULL);
	//create thread that runs on print_Ys

	for(n=100;n>0;n--){
		fputc('Z',stdout); usleep(1);
	}
	
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);


	return 0;
}
