#include <stdio.h>
#include "signal_timer.h"

extern int wait;

int main()
{
  init_counts();
  while (wait)
  {
    waiting();
  }
  return 0;
}