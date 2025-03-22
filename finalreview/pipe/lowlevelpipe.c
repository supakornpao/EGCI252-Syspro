#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipefd[2], n_bytes;
  char buffer[BUFSIZ];
  char *str;

  memset(buffer, '\0', sizeof(buffer));

  sscanf(argv[1] ,"%s",buffer);

  if (pipe(pipefd) == 0) {
    n_bytes = write(pipefd[1], buffer, strlen(buffer));
    n_bytes = read(pipefd[0], buffer, BUFSIZ);
    printf("Size of string: %d\n", n_bytes);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}