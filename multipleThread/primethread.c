#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define max 50000
#define N 5


struct range{
	unsigned long start;
	unsigned long end;
	unsigned long count;
};


void* compute_prime(void* arg){
	int *primelist = malloc((max/N)*sizeof(int)); 
	if(!primelist){
		perror("Memory allocation failed");
		pthread_exit(NULL);
	}

	struct range *n = (struct range*)arg;
	unsigned long candidate;
	n->count = 0;
	if(n->start<2) n->start = 2;

	candidate = n->start;
	while(candidate<=n->end){
		int factor, is_prime = 1;
		for(factor = 2; factor*factor<=candidate && is_prime == 1;factor++)
			if(candidate%factor==0)
				{is_prime = 0;break;}

		if(is_prime){
			primelist[n->count] = candidate;
			//primelist[count] = candidate;
			n->count++;
		}
		candidate++;
	}
	primelist = realloc(primelist,(n->count+1)*sizeof(int));
	if(!primelist){
		perror("Memory reallocation failed");
		pthread_exit(NULL);
	}
	primelist[n->count] = -1;//end of the list
	return primelist;
}

int main(){
	pthread_t threads[N];
	struct range *args[N];
	int thread;
	int *count[N];

	
	unsigned long start = 0;
	unsigned long end = start+max;
	unsigned long number_per_thread = max/N;
	//number of prime per thread

	assert(max % N == 0);

	for(thread = 0; thread<N;thread++){
		args[thread] = malloc(sizeof(struct range));
		if(!args[thread]){
			perror("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		args[thread]->start = start+(thread* number_per_thread);
		args[thread]->end = args[thread]->start + number_per_thread - 1;
		assert(pthread_create(&threads[thread],NULL,compute_prime,args[thread])==0);
	}

	unsigned long total_number = 0;

	for(thread = 0; thread<N;thread++){
		void *result;
		pthread_join(threads[thread],&result);
		count[thread] = (int*)result;
		int prime_count=0;
		while(count[thread][prime_count]!=-1){
			prime_count++;
		}

		total_number += prime_count;
		free(args[thread]);
	}

	for(int i=0;i<N;i++){
		int *primelist = count[i];
		printf("\nList of primes from thread %d: \n",i+1);
		for(int j=0;primelist[j]!=-1;j++){
			printf("%d  ",primelist[j]);
		}
		printf("\n");
		printf("=========================================\n");
		free(primelist);
	}

	

	printf("\nTotal number of primes less than %ld: %ld.\n", end, total_number);
	return 0;
}
