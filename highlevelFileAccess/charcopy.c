#include <stdio.h>

int main(void){
	FILE *in, *out;
	char c;

	in = fopen("file.in","r");
	out = fopen("file.out","w");

	while((c=fgetc(in))!=EOF){
		fputc(c,out);
	}

	fclose(in);
	fclose(out);
	return 0;
}
