#define _DEFAULT_SOURCE
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

static FILE *fptr;
extern sem_t *mutex;
extern sem_t *full;

void read_line() {
  char buffer[100] = {0};
  int c;
  while ((c = fgetc(fptr)) != EOF && c != '\n') {
    sprintf(buffer + strlen(buffer), "%c", c);
  }
  if (strlen(buffer) == 0) {
    return;
  }
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
  fptr = fopen(FILENAME, "r");
  if (fptr == NULL) {
    perror("Error occured while opening file\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < LINE_COUNT; i++) {
    sem_wait(full);
    sem_wait(mutex);
    read_line();
    sem_post(mutex);
    usleep(1000 * 100);
  }

  fclose(fptr);
}