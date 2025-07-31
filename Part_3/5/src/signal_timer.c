#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "signal_timer.h"

#define BUFFER_LEN 100

static counters counts;
int wait = 1;
static int file;
static char filename[] = "output_file.txt";

static char buffer[BUFFER_LEN + 1];

void init_counts()
{
  counts.cur_time = 0;
  counts.sigint_count = 0;

  file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  signal(SIGINT, sigint_call);
  signal(SIGQUIT, sigint_call);
}

void sigint_call(int signal_code)
{
  switch (signal_code)
  {
  case SIGINT:
    signal(SIGINT, sigint_call);
    counts.sigint_count++;
    snprintf(buffer, BUFFER_LEN, "Got sigint #%d\n", counts.sigint_count);
    write(file, buffer, strlen(buffer));
    if (counts.sigint_count > 2)
    {
      snprintf(buffer, BUFFER_LEN, "Ending due to SIGINT\n");
      write(file, buffer, strlen(buffer));
      wait = 0;
      close(file);
      return;
    }
    break;

  case SIGQUIT:

    snprintf(buffer, BUFFER_LEN, "Ending due to SIGQUIT\n");
    write(file, buffer, strlen(buffer));
    wait = 0;
    close(file);
    return;

  default:
    snprintf(buffer, BUFFER_LEN, "Unknown signal\n");
    write(file, buffer, strlen(buffer));
    break;
  }
  return;
}

void waiting()
{
  snprintf(buffer, BUFFER_LEN, "%d\n", counts.cur_time);
  write(file, buffer, strlen(buffer));
  counts.cur_time++;
  sleep(1);
}