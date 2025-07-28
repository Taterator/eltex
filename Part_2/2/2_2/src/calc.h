#ifndef CALC_H_
#define CALC_H_

int add(float num_1, float num_2, float *res);
int add_n(int count, float *res, ...);
int subtract(float num_1, float num_2, float *res);
int multiply(float num_1, float num_2, float *res);
int divide(float num_1, float num_2, float *res);

#endif