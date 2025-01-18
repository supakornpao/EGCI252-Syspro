#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>

int main(void){
	char block[8192];
	int in, out, nread;
	int length = 0;
	char nullchar = '\0';

	in = open("file.in",O_RDONLY);
	out = open("file.out",O_WRONLY| O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);

	while((nread = read(in, block,sizeof(block)))>0){
		write(out, block, nread);
		length+=nread;
	}

	
	int start = ((length)/2)-5; //middle part minus half of n (starting point)
	lseek(out,start, SEEK_SET);
	
	for(int i = 0; i<10;i++){ // replace n element from integer start
		write(out,&nullchar,1);
	}
	
	return 0;
}
