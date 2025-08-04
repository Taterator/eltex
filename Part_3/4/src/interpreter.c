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

void print_command(char *args[], int args_count)
{
  printf("> Printing args\n> ");
  for (int i = 0; i < args_count; i++)
  {
    printf("%s ", args[i]);
  }
  printf("\n");
}

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

int perform_command(char *args[], int *args_count, int prev_fd[2])
{
  int divider_flag = 0;
  char *cur_command[ARGS_COUNT];
  int cur_counter = 0;
  char *later_command[ARGS_COUNT];
  int later_counter = 0;
  init_arr(cur_command);
  init_arr(later_command);

  for (int i = 0; i < *args_count; i++)
  {
    // printf("%s %d\n", args[i], strcmp(args[i], "|"));
    if (strcmp(args[i], "|") == 0 && divider_flag == 0)
    {
      // printf("found divider |\n");
      // printf("%d %d\n", getpid(), getppid());
      divider_flag = 1;
      continue;
    }
    if (divider_flag)
    {
      later_command[later_counter++] = strdup(args[i]);
    }
    else
    {
      cur_command[cur_counter++] = strdup(args[i]);
    }
  }

  pid_t pid;
  int rv;
  int is_pipe = 0;
  if (prev_fd[0] == -1 && prev_fd[1] == -1)
  {
    // no pipe was passed
  }
  else
  {
    is_pipe = 1;
  }
  int fd[2];
  if (pipe(fd) == -1)
  {
    return 1;
  }

  switch (pid = fork())
  {
  case -1:
    perror("Unable to fork");
    exit(EXIT_FAILURE);
  case 0: /* Потомок */
    int ret;
    if (divider_flag)
    {
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      close(fd[1]);
    }
    if (is_pipe)
    {
      dup2(prev_fd[0], STDIN_FILENO);
      close(prev_fd[0]);
      close(prev_fd[1]);
    }
    ret = execvp(cur_command[0], cur_command);
    if (ret == -1)
      parse_error(errno);

    _exit(EXIT_SUCCESS);

  default:

    wait(&rv);
    if (divider_flag)
    {
      perform_command(later_command, &later_counter, fd);
    }
    close(fd[0]);
    close(fd[1]);
    free_arr(cur_command, &cur_counter);
    free_arr(later_command, &later_counter);
    return 0;
  }
  return 0;
}

int interpreter()
{
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
    int fd[2] = {-1, -1};
    perform_command(arr, &arr_arg_count, fd);
    free_arr(arr, &arr_arg_count);
    printf("-> ");
    fgets(buffer, MAX_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    token = strtok(buffer, " ");
  }
  free_arr(arr, &arr_arg_count);

  exit(EXIT_SUCCESS);
  return 0;
}