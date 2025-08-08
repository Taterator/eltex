#define _DEFAULT_SOURCE
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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

void read_line() {
  char buffer[BUF_SIZE] = {0};
  strcpy(buffer, shmp->buf);
  int nums[11];
  int count = 0;
  char *buff_ptr = strtok(buffer, " ");
  while (buff_ptr != NULL) {
    nums[count++] = atoi(buff_ptr);
    buff_ptr = strtok(NULL, " ");
  }
  int min = nums[0];
  int max = nums[0];

  for (int i = 0; i < count; i++) {
    if (nums[i] > max) max = nums[i];
    if (nums[i] < min) min = nums[i];
  }
  printf("Min = %d, max = %d\n", min, max);
}

void reader() {
  run = 1;
  signal(SIGINT, sigint_call);
  while (run) {
    sem_wait(full);
    sem_wait(mutex);
    read_line();
    sem_post(mutex);
    usleep(1000 * 100);
  }
}