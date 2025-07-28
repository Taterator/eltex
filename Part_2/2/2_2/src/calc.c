#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "calc.h"

int add(float num_1, float num_2, float *res)
{
  *res = num_1 + num_2;
  return 0;
}

int add_n(int count, float *res, ...)
{
  *res = 0;
  va_list args;
  va_start(args, res);

  for (int i = 0; i < count; i++)
  {
    *res += va_arg(args, double);
  }

  va_end(args);

  return 0;
}

int subtract(float num_1, float num_2, float *res)
{
  *res = num_1 + num_2;
  return 0;
}

int multiply(float num_1, float num_2, float *res)
{
  *res = num_1 * num_2;
  return 0;
}

int divide(float num_1, float num_2, float *res)
{
  *res = num_1 / num_2;
  return 0;
}
