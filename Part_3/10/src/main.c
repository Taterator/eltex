#define _POSIX_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

sem_t *mutex;
sem_t *full;
struct shmseg *shmp;

int main() {
  pid_t pid;
  int rv;

  int shmid;
  shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Shared memory");
    return 1;
  }

  // Attach to the segment to get a pointer to it.
  shmp = shmat(shmid, NULL, 0);
  if (shmp == (void *)-1) {
    perror("Shared memory attach");
    return 1;
  }

  mutex = sem_open("sem_1", O_CREAT, 0666, 1);
  full = sem_open("sem_2", O_CREAT, 0666, 0);

  shmp->complete = 0;

  switch (pid = fork()) {
    case -1:
      perror("Unable to fork");
      exit(EXIT_FAILURE);
    case 0: /* Потомок */
      reader();
      _exit(EXIT_SUCCESS);

    default:
      printer();
      kill(pid, SIGINT);
      wait(&rv);
  }
  printf("Total count: %d\n", shmp->complete);
  sem_close(mutex);
  sem_close(full);
  sem_unlink("sem_1");
  sem_unlink("sem_2");

  if (shmdt(shmp) == -1) {
    perror("shmdt");
    return 1;
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("shmctl");
    return 1;
  }

  return 0;
}
