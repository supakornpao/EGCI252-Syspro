#include <malloc.h>
#include <pthread.h>
#include <stdio.h>

static pthread_key_t thread_log_key;

//Write MESSAGE to the log file for the current thread.

void write_to_thread_log(const char* message){
	FILE* thread_log = (FILE*)pthread_getspecific(thread_log_key);

	fprintf(thread_log,"%s\n",message);
}

void close_thread_log(void* thread_log){
	fclose((FILE*)thread_log);
}

void* thread_function(void* arg){
	char thread_log_filename[30];
	FILE* thread_log;

	//Generate the filename for this thread's log file
	sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());

	//Open the log file
	thread_log = fopen(thread_log_filename,"w");

	//store the pointer in thread-specific data under thread_log_key
	pthread_setspecific(thread_log_key, thread_log);
	write_to_thread_log("Thread starting.");

	return NULL;
}


int main(){
	int i;
	pthread_t threads[5];

	pthread_key_create(&thread_log_key, close_thread_log);

	for(i=0;i<5;i++){
		pthread_create(&(threads[i]),NULL,thread_function,NULL);
	}

	for(i=0;i<5;i++){
		pthread_join(threads[i],NULL);
	}

	return 0;
}
