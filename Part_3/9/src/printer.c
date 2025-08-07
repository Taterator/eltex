#define _DEFAULT_SOURCE
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

static FILE *fptr;
extern sem_t *mutex;
extern sem_t *full;

void write_random_nums() {
  int count = 1 + rand() % 10;
  int num = 0;
  for (int i = 0; i < count; i++) {
    num = rand() % 20 - 10;
    fprintf(fptr, "%d ", num);
  }
  fprintf(fptr, "\n");
  fflush(fptr);
}

void printer() {
  srand(time(NULL));

  fptr = fopen(FILENAME, "w");
  if (fptr == NULL) {
    perror("Error occured while opening file\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < LINE_COUNT; i++) {
    sem_wait(mutex);
    write_random_nums();
    sem_post(mutex);
    sem_post(full);

    usleep(1000 * 100);
  }

  fclose(fptr);
}