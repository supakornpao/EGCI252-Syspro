#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void cleanup(void* arg){
	printf("Thread %u: cleaning up\n", (unsigned int) pthread_self());
//	printf("Cleanup: Releasing resource: %s\n",(char*)arg);
	free(arg);
}

void* thread_func(void* arg){
	int *p = malloc(32);

	printf("Thread started.\n");

	//Register cleanup handler
	pthread_cleanup_push(cleanup, p);
	
	//Simulate some work
	for(int i=0;i<5;i++){
		printf("Thread working: %d\n",i);
		sleep(1);
		//simulate a cancellation point
	}

	//Deregister cleanup handler (don't execute it now)
	pthread_cleanup_pop(0);

	printf("Thread finished normally.\n");
	return NULL;
}

int main(void){
	pthread_t thread;

	//create new thread
	if(pthread_create(&thread,NULL,thread_func,NULL)!=0){
		perror("Failed to create thread");
		return 1;
	}
	
	//Let the thread run for a bit
	sleep(2);

	//cancel the thread
	printf("Main: Canceling thread..\n");
	pthread_cancel(thread);

	//Wait for the thread to finish
	pthread_join(thread,NULL);

	printf("Main: Thread joined.\n");

	return 0;

}
