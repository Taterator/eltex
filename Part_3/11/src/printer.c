#define _DEFAULT_SOURCE
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

static int run;

extern sem_t *mutex;
extern sem_t *full;
extern struct shmseg *shmp;

static void sigint_call(int signal_code) {
  switch (signal_code) {
    case SIGINT:
      run = 0;
      break;

    default:
      break;
  }
  return;
}

int write_random_nums() {
  char buffer[BUF_SIZE] = {0};
  int count = 1 + rand() % 10;
  int num = 0;
  memset(shmp->buf, '\0', BUF_SIZE - 1);
  for (int i = 0; i < count; i++) {
    num = rand() % 20 - 10;
    sprintf(buffer + strlen(buffer), "%d ", num);
  }
  memcpy(shmp->buf, buffer, strlen(buffer));
  return strlen(shmp->buf);
}

void printer() {
  srand(time(NULL));
  run = 1;
  signal(SIGINT, sigint_call);
  while (run) {
    sem_wait(mutex);
    shmp->cnt = write_random_nums();
    shmp->complete++;
    sem_post(mutex);
    sem_post(full);

    usleep(1000 * 100);
  }
}