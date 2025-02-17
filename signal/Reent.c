#include<stdio.h>
#include<pthread.h>
int g_var = 1;

int f(void *p){
	int *x = (int *)p;
	*x = *x + 2;
	return *x;
}

void *g(void *p){
	printf("p_var = %d \n",f(p)+2);
}

int main(){
	
	pthread_t t1,t2;
	int p = 1,q = 1;
	pthread_create(&t1,NULL,g,&p);
	pthread_create(&t2,NULL,g,&q);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
