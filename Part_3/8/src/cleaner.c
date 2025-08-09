#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

key_t key;
int semid;

void free_sem() {
  if (semctl(semid, 0, IPC_RMID) == -1) {
    perror("semctl free");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    exit(EXIT_FAILURE);
  }

  if ((key = ftok(argv[1], 1)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  if ((semid = semget(key, 3, 0666 | IPC_CREAT)) == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  free_sem();
  return 0;
}