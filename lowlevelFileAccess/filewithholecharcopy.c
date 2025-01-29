#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int main(void){
	char c;
	char nullchar = '\0';
	int in, out;
	int length = 0;

	in = open("file.in",O_RDONLY);
	out = open("file.out",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);


	while(read(in,&c,1)==1){
	       	write(out,&c,1);
		length++;
	}
	int starting = (length/2)-5;
	lseek(out,starting, SEEK_SET);

	for(int i = 0; i<10;i++){
		write(out,&nullchar,1);
	}

	return 0;
}
