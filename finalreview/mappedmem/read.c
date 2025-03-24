#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>

#define MAPPED "/tmp/mfile"
#define FILE_LENGTH 0x100

int main(){
    int fd;
    void* file_memory;
    char buffer[BUFSIZ];
    fd = open(MAPPED,O_RDWR,S_IRUSR|S_IWUSR);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    file_memory = mmap(0,FILE_LENGTH,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (file_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    strncpy(buffer, (char*)file_memory, BUFSIZ);
    printf("%s",buffer);
    munmap(file_memory,FILE_LENGTH);
    return 0;
}