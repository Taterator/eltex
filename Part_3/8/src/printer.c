#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

static FILE *fptr;
static key_t key;
static int semid;

void free_sem() {
  union semun arg;
  int fa_count = semctl(semid, 3, GETVAL, arg);
  if (fa_count < 0) {
    perror("semctl free");
    exit(EXIT_FAILURE);
  } else if (fa_count == 0) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
      perror("semctl free");
      exit(EXIT_FAILURE);
    }
  }
}

void shutdown() {
  fclose(fptr);
  if (semop(semid, &down_fa, 1) == -1) {
    perror("semop prod");
    exit(EXIT_FAILURE);
  }
  if (semop(semid, &down_prod, 1) == -1) {
    perror("semop prod");
    exit(EXIT_FAILURE);
  }
  free_sem();
}

void sigint_call(int signal_code) {
  switch (signal_code) {
    case SIGINT:
      shutdown();
      exit(EXIT_SUCCESS);
      break;

    default:
      break;
  }
  return;
}

void write_random_nums() {
  if (semop(semid, &lock_res, 1) == -1) {
    perror("semop file lock");
    shutdown();
    exit(EXIT_FAILURE);
  }
  int count = 1 + rand() % 10;
  int num = 0;
  for (int i = 0; i < count; i++) {
    num = rand() % 20 - 10;
    fprintf(fptr, "%d ", num);
  }
  fprintf(fptr, "\n");
  fflush(fptr);
  if (semop(semid, &rel_res, 1) == -1) {
    perror("semop file rel");
    shutdown();
    exit(EXIT_FAILURE);
  }
  sleep(1);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_call);
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));

  fptr = fopen(argv[1], "a");

  if ((key = ftok(argv[1], 1)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  if ((semid = semget(key, 4, 0666 | IPC_CREAT)) == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  // 0 - file access, 1 - sem init
  // 2 - amount of producers, 3 - file access count
  union semun arg;
  int init = semctl(semid, 1, GETVAL, arg);
  if (init == 0) {  // Initialise semaphores
    printf("Init sems\n");
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {  // Init file access
      perror("semctl");
      exit(EXIT_FAILURE);
    }
    arg.val = 1;
    if (semctl(semid, 1, SETVAL, arg) == -1) {  // Init init
      perror("semctl");
      exit(EXIT_FAILURE);
    }
    arg.val = 0;
    if (semctl(semid, 2, SETVAL, arg) == -1) {  // Init producers
      perror("semctl");
      exit(EXIT_FAILURE);
    }
    arg.val = 0;
    if (semctl(semid, 3, SETVAL, arg) == -1) {  // Init file count
      perror("semctl");
      exit(EXIT_FAILURE);
    }
  } else if (init < 0) {
    perror("semctl");
    exit(EXIT_FAILURE);
  }

  if (semop(semid, &up_prod, 1) == -1) {
    perror("semop prod");
    exit(EXIT_FAILURE);
  }
  if (semop(semid, &up_fa, 1) == -1) {
    perror("semop prod");
    exit(EXIT_FAILURE);
  }
  while (1) {
    write_random_nums();
  }

  shutdown();
  return 0;
}