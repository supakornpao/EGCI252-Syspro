#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

struct char_print_parms{
	char character;
	int count;
};

void *char_print(void* parameters){
	struct char_print_parms* p = (struct char_print_parms*) parameters;
	int i;
	for(i=0;i<p->count;++i){
		fputc(p->character, stdout);
	}
	return NULL;
}

int main(){
	int n;
	pthread_t thread1_id, thread2_id;

	struct char_print_parms thread1_args;
	struct char_print_parms thread2_args;
	
	thread1_args.character = 'x';
	thread1_args.count = 30000;
	pthread_create(&thread1_id,NULL,&char_print,&thread1_args);
	
	thread2_args.character = '0';
	thread2_args.count = 20000;
	pthread_create(&thread2_id,NULL,&char_print,&thread2_args);
	
	pthread_join(thread1_id, NULL);
	pthread_join(thread2_id, NULL);


	return 0;
}
