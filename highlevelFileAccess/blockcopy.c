#include <stdio.h>
#define BUFFER_SIZE 512

int main(void){
	FILE *in,*out;
	
	char buffer[BUFFER_SIZE];

	in = fopen("file.in","r");
	out = fopen("file.out","w");

	while(fgets(buffer,BUFFER_SIZE,in)!=NULL)
		fputs(buffer,out);

	fclose(in);
	fclose(out);
	return 0;
}
