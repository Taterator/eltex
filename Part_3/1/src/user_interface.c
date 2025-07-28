#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "arg_parser.h"

int main(int argc, char *argv[])
{
  pid_t pid;
  int rv;
  switch (pid = fork())
  {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0: /* Потомок */
    child_parser(argc, argv);
    _exit(EXIT_SUCCESS);

  default: /* Родитель */
    parent_parser(argc, argv);
    wait(&rv);
    exit(EXIT_SUCCESS);
  }
  return 0;
}