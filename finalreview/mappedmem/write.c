#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100
#define MAPPED "/tmp/mfile"

int main(){
    int fd;
    void* file_memory;
    char buffer[BUFSIZ];

    fd = open(MAPPED, O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    lseek(fd,FILE_LENGTH+1,SEEK_SET);
    write(fd,"",1);
    lseek(fd,0,SEEK_SET);

    file_memory = mmap(0,FILE_LENGTH,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    close(fd);
    printf("Enter data: ");
    fgets(buffer,BUFSIZ,stdin);
    buffer[BUFSIZ-1]='\0';
    sprintf((char*)file_memory,"%s",buffer);
    munmap(file_memory,FILE_LENGTH);
    return 0;
}