#ifndef CALC_H_
#define CALC_H_

typedef struct
{
  int (*operation)(float, float, float *);
  char *name;

} op_struct;

int add(float num_1, float num_2, float *res);
int subtract(float num_1, float num_2, float *res);
int multiply(float num_1, float num_2, float *res);
int divide(float num_1, float num_2, float *res);

#endif