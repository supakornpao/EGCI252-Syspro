#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void){
	char block[512];
	int in, out, nread;

	in = open("file.in",O_RDONLY);
	out = open("file.out",O_WRONLY);

	while((nread=read(in,block,sizeof(block)))>0)
		write(out,block,nread);
	return 0;
}

