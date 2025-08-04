#include "messenger.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("No arg\n");
    return 0;
  }

  if (strcmp(argv[1], "1") == 0)
  {
    ping();
  }
  else if (strcmp(argv[1], "2") == 0)
  {
    pong();
  }
  else
  {
    printf("Wrong arg\n");
  }

  return 0;
}