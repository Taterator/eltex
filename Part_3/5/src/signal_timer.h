#ifndef TIMER_H_
#define TIMER_H_

typedef struct
{
  int cur_time;
  int sigint_count;
} counters;

void init_counts();
void sigint_call(int signal);
void waiting();

#endif