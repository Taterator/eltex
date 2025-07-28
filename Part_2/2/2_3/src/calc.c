#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "calc.h"

int add(float num_1, float num_2, float *res)
{
  *res = num_1 + num_2;
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
