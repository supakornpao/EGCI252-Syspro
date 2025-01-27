#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void){
	char block[512];
	int filein, fileout, nread;

	filein = open("file.in",O_RDONLY);
	fileout = open("file.out",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);

	while((nread = read(filein, block, sizeof(block)))>0)
		write(fileout,block,nread);
}
