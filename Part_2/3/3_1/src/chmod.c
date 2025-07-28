#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "chmod.h"

int parse_flags_chars(char *str, flags_t *flags);
int parse_flags_nums(char *str, flags_t *flags);
int modify_flags(flags_t *flags);

int parse_flags(char *str, flags_t *flags)
{
  flags->person = 0;
  flags->permission = 0;
  flags->result = 0;
  flags->action = NONE_ACTION;
  int len = strlen(str);
  if (len < 3)
    return 1;
  if (isdigit(str[0]))
  {
    parse_flags_nums(str, flags);
    modify_flags(flags);
  }
  else
  {
    parse_flags_chars(str, flags);
    modify_flags(flags);
  }
  return 0;
}

int parse_flags_nums(char *str, flags_t *flags)
{
  int len = strlen(str);
  if (len > 3)
  {
    printf("Error: too many numeric args\n");
    return 1;
  }
  flags->action = PUT;
  for (int i = 0; i < len; i++)
  {
    int num = str[i] - '0';
    num <<= (len - i - 1) * 3;
    flags->result += num;
  }
  return 0;
}

int parse_flags_chars(char *str, flags_t *flags)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    switch (str[i])
    {
    case 'u':
      flags->person |= S_IRWXU;
      break;
    case 'g':
      flags->person |= S_IRWXG;
      break;
    case 'o':
      flags->person |= S_IRWXO;
      break;
    case 'a':
      flags->person |= (S_IRWXU | S_IRWXG | S_IRWXO);
      break;
    case '+':
      flags->action = ADD;
      break;
    case '-':
      flags->action = REMOVE;
      break;
    case '=':
      flags->action = PUT;
      break;
    case 'r':
      flags->permission |= (S_IRUSR | S_IRGRP | S_IROTH);
      break;
    case 'w':
      flags->permission |= (S_IWUSR | S_IWGRP | S_IWOTH);
      break;
    case 'x':
      flags->permission |= (S_IXUSR | S_IXGRP | S_IXOTH);
      break;
    default:
      printf("Error :Unknown symbol\n");
      return 1;
      break;
    }
    flags->result = flags->person & flags->permission;
  }
  return 0;
}

int modify_flags(flags_t *flags)
{
  switch (flags->action)
  {
  case ADD:
    flags->result |= flags->original;
    break;

  case REMOVE:
    flags->result = flags->original & (~flags->result);
    break;

  case PUT:
    flags->result = flags->result;
    break;

  default:
    printf("Error: Unknown action\n");
    return 1;
  }
  return 0;
}