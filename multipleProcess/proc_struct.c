#include<stdlib.h>
#include<stdio.h>

int x = 100;
char y = 'A';
int z;

int func(int a){
	int *b, *c;

	b = malloc(sizeof(int));
	c = malloc(sizeof(int));

	*b = 3; *c = 2;

	printf("<Heap Segment>\n");
	printf("Address of *b = %p\n",b);
	printf("Data of *b = %d\n",*b);
	printf("Address of *c = %p\n",c);
	printf("Data of *c = %d\n",*c);

	a += x+3;

	printf("<Stack Segment>\n");
	printf("Address of c = %p\n",&c);
	printf("Data of c = %p\n",c);
	printf("Address of b = %p\n",&b);
	printf("Data of b = %p\n", b);
	printf("Address of a = %p\n", &a);
	printf("Data of a = %d\n", a);
	printf("Address of RBP = %p\n", &a+2);
	printf("Data of RBP = 0x%x%08x\n",*(&a+2),*(&a+1));
	printf("Address of rAdd = %p\n",&a+4);
	printf("Data of rAdd = 0x%x%08x\n",*(&a+4),*(&a+3));

	while(1);

	return a;
}

int main(void){
	printf("<Test Segment>\n");
	printf("Address of main = %p\n",main);
	printf("Address of func = %p\n",func);

	printf("<Data Segment>\n");
	printf("<<Initialized Data>>\n");
	printf("Address of x = %p\n",&x);
	printf("Data of x = %d\n", x);
	printf("Address of y = %p\n",&y);
	printf("Data of y = %c\n", y);
	printf("<<Uninitialized Data>>\n");
	printf("Address of z = %p\n",&z);
	printf("Data if z = %d\n",z);

	x = func(5);
	printf("[x=%d]\n",x);

	return 0;
	
}
