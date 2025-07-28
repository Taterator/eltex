#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "chmod.h"

void print_permissions(mode_t mode)
{
  // mode &= 0777;
  printf("Literal: ");
  (mode & S_IRUSR) == S_IRUSR ? printf("r") : printf("-");
  (mode & S_IWUSR) == S_IWUSR ? printf("w") : printf("-");
  (mode & S_IXUSR) == S_IXUSR ? printf("x") : printf("-");

  (mode & S_IRGRP) == S_IRGRP ? printf("r") : printf("-");
  (mode & S_IWGRP) == S_IWGRP ? printf("w") : printf("-");
  (mode & S_IXGRP) == S_IXGRP ? printf("x") : printf("-");

  (mode & S_IROTH) == S_IROTH ? printf("r") : printf("-");
  (mode & S_IWOTH) == S_IWOTH ? printf("w") : printf("-");
  (mode & S_IXOTH) == S_IXOTH ? printf("x") : printf("-");
  printf("\n");

  printf("Numeric: ");
  printf("%o\n", mode & 0777);

  // printf("In bits: ");
  // printf("%b\n", mode);

  printf("\n");
}

int main(int argc, char *argv[])
{
  // chmod [ugoa][+-=][rwx] dir
  // chmod [0-7]x3 dir
  flags_t flags;
  struct stat buffer;
  int status;
  printf("argc = %d\n", argc);
  status = stat(argv[argc - 1], &buffer);
  if (status)
  {
    printf("Nooo\n");
    exit(1);
  }
  flags.original = buffer.st_mode;

  printf("Original flags:\n");
  print_permissions(flags.original);

  if (argc > 2)
  {
    printf("Modified flags:\n");
    parse_flags(argv[1], &flags);
    print_permissions(flags.result);
  }

  return 0;
}