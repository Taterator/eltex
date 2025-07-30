#include "interpreter.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_LENGTH 50
#define ARGS_COUNT 20

char *strdup(const char *src)
{
  char *dst = malloc(strlen(src) + 1); // Space for length plus nul
  if (dst == NULL)
    return NULL;    // No memory
  strcpy(dst, src); // Copy the characters
  return dst;       // Return the new string
}

void parse_error(int error)
{
  switch (error)
  {
  case ENOTDIR:
    printf("Error: Not a directory\n"); //???
    break;
  case ENOENT:
    printf("Error: No such file exists\n");
    break;
  case EACCES:
    printf("Error: Permission denied\n");
    break;
  default:
    printf("Other error: %d\n", error);
    break;
  }
}

void init_arr(char *arr[])
{
  for (int i = 0; i < ARGS_COUNT; i++)
  {
    arr[i] = NULL;
  }
}

void free_arr(char *arr[], int *count)
{
  for (int i = 0; i < *count; i++)
  {
    free(arr[i]);
    arr[i] = NULL;
  }
  *count = 0;
}

int interpreter()
{
  pid_t pid;
  int rv;
  char buffer[MAX_LENGTH + 1];
  char *arr[ARGS_COUNT]; // If uninit, causes EFAULT when passed
  init_arr(arr);
  int arr_arg_count = 0;
  printf("type q to quit\n");
  printf("-> ");
  fgets(buffer, MAX_LENGTH, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  char *token = strtok(buffer, " ");
  while (strcmp(buffer, "q") != 0)
  {

    while (token)
    {
      arr[arr_arg_count++] = strdup(token);
      token = strtok(NULL, " ");
    }

    switch (pid = fork())
    {
    case -1:
      perror("Unable to fork");
      exit(EXIT_FAILURE);
    case 0: /* Потомок */
      int ret;
      ret = execvp(arr[0], arr);
      if (ret == -1)
        parse_error(errno);

      _exit(EXIT_SUCCESS);

    default:
      wait(&rv);
      free_arr(arr, &arr_arg_count);
      printf("-> ");
      fgets(buffer, MAX_LENGTH, stdin);
      buffer[strcspn(buffer, "\n")] = 0;
      token = strtok(buffer, " ");
    }
  }
  free_arr(arr, &arr_arg_count);

  exit(EXIT_SUCCESS);
  return 0;
}