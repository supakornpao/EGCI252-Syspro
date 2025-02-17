#include<stdio.h>
#include<pthread.h>
int g_var = 1;

int f(){
	g_var = g_var+2;
	return g_var;
}

void *g(void *p){
	printf("p_var = %d \n",f()+2);
}

int main(){
	
	pthread_t t1,t2;
	pthread_create(&t1,NULL,g,NULL);
	pthread_create(&t2,NULL,g,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
