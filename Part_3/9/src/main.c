#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

sem_t *mutex;
sem_t *full;

int main() {
  pid_t pid;
  int rv;
  mutex = sem_open("sem_1", O_CREAT, 0666, 1);
  full = sem_open("sem_2", O_CREAT, 0666, 0);

  switch (pid = fork()) {
    case -1:
      perror("Unable to fork");
      exit(EXIT_FAILURE);
    case 0: /* Потомок */
      reader();
      _exit(EXIT_SUCCESS);

    default:
      printer();
      wait(&rv);
      sem_close(mutex);
      sem_close(full);
      sem_unlink("sem_1");
      sem_unlink("sem_2");
  }

  return 0;
}