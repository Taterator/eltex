#ifndef CHMOD_H_
#define CHMOD_H_

#include <sys/types.h>
#include <sys/stat.h>

#define READ 04
#define WRITE 02
#define EXECUTE 01

typedef enum
{
  USER,
  GROUP,
  OTHER,
  ALL,
  NONE_PERSON
} person_t;

typedef enum
{
  ADD,
  REMOVE,
  PUT,
  NONE_ACTION
} action_t;

typedef struct
{
  mode_t original;
  mode_t person;
  mode_t permission;
  action_t action;
  mode_t result;
} flags_t;

int parse_flags(char *str, flags_t *flags);

#endif