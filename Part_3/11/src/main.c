#define _DEFAULT_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
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

  int shmid = shm_open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (shmid == -1) {
    perror("Shared memory");
    return 1;
  }

  if (ftruncate(shmid, sizeof(struct shmseg)) == -1) {
    perror("Shared memory truncate");
    return 1;
  }

  shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ | PROT_WRITE, MAP_SHARED,
              shmid, 0);
  if (shmp == MAP_FAILED) {
    perror("Shared memory map");
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

  if (close(shmid) == -1) {
    perror("shm close");
    return 1;
  }

  if (shm_unlink(FILENAME) == -1) {
    perror("shm unlink");
    return 1;
  }

  return 0;
}
