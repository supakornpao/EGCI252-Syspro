#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void){
	char c;
	int filein,fileout;
	
	filein = open("file.in",O_RDONLY);
	fileout = open("file.out",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	while(read(filein,&c,1)==1) write(fileout,&c,1);
	
	return 0;

}

