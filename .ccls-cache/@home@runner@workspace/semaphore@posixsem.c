//this one is named posix

#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int i;
  int pause_time = 800;
  char op_char = 'O';
  sem_t *mysem;
  mysem = sem_open("/mysem", O_CREAT | O_RDWR, 0660, 1);
  if (argc > 1) {
    op_char = 'X';
    usleep(pause_time);
  }
  for (i = 0; i < 10; i++) {
    if (sem_wait(mysem) == -1)
      exit(EXIT_FAILURE);

    // Begining of the critical section
    printf("%c", op_char);
    fflush(stdout);
    usleep(pause_time);
    printf("%c", op_char);
    fflush(stdout);
    // End of the critical section
    if (sem_post(mysem) == -1)
      exit(EXIT_FAILURE);
    usleep(pause_time);
  }
  printf("\n%d - finished\n", getpid());
  if (argc > 1) {
    usleep(pause_time);
    sem_unlink("/mysem");
  }
  exit(EXIT_SUCCESS);
}