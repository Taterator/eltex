#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

#include "arg_parser.h"

int is_a_number(char *num_str)
{
  int dot_count = 0;
  int len = strlen(num_str);
  for (int i = 0; i < len; i++)
  {
    if (isdigit(num_str[i]) || num_str[i] == '.')
    {
      if (num_str[i] == '.')
      {
        dot_count++;
        if (dot_count > 1)
          return 0;
      }
    }
    else
    {
      return 0;
    }
  }
  return 1;
}

// Для тестирования парсера
void parser(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    printf("[%d] = %s, {%d}\n", i, argv[i], is_a_number(argv[i]));
    if (is_a_number(argv[i]))
    {
      printf("%.4f\n", atof(argv[i]) * 2);
    }
  }
}

void child_parser(int argc, char *argv[])
{
  for (int i = 1; i < argc; i += 2)
  {
    if (is_a_number(argv[i]))
    {
      printf("%.4f\n", atof(argv[i]) * 2);
    }
    else
    {
      printf("%s\n", argv[i]);
    }
  }
}

void parent_parser(int argc, char *argv[])
{
  for (int i = 2; i < argc; i += 2)
  {
    if (is_a_number(argv[i]))
    {
      printf("%.4f\n", atof(argv[i]) * 2);
    }
    else
    {
      printf("%s\n", argv[i]);
    }
  }
}