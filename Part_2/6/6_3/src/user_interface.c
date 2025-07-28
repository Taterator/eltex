#include <stdio.h>
#include "calc.h"
#include <math.h>
#include <dlfcn.h>
#include <stdlib.h>

#define OPER_COUNT 4

void print_result(float res)
{
  printf("-> %.3f\n", res);
}

int get_command_interface(op_struct operations[OPER_COUNT])
{
  printf("| Enter command:\n");
  for (int i = 0; i < OPER_COUNT; i++)
  {
    printf("| %d - %s\n", i + 1, operations[i].name);
  }
  printf("| quit - to exit\n");
  int num = -1;
  printf("> ");
  scanf("%d", &num);
  return num;
}

void get_numbers_inteface(float *num_1, float *num_2)
{
  printf("| Enter first number:\n");
  scanf("%f", num_1);
  printf("| Enter second number:\n");
  scanf("%f", num_2);
}

void print_operation(op_struct operation)
{
  float num_1 = 0;
  float num_2 = 0;
  float res = 0;
  int error = 0;
  printf("| You chose \"%s\"\n", operation.name);
  get_numbers_inteface(&num_1, &num_2);
  error = operation.operation(num_1, num_2, &res);
  if (!error)
    print_result(res);
}

int main()
{
  void *handle_add, *handle_sub, *handle_mul, *handle_div;
  int (*add_op)(float, float, float *);
  int (*sub_op)(float, float, float *);
  int (*mul_op)(float, float, float *);
  int (*div_op)(float, float, float *);
  char *error;
  handle_add = dlopen("./libadd.so", RTLD_LAZY);
  handle_sub = dlopen("./libsub.so", RTLD_LAZY);
  handle_mul = dlopen("./libmul.so", RTLD_LAZY);
  handle_div = dlopen("./libdiv.so", RTLD_LAZY);
  if (!handle_add || !handle_sub || !handle_mul || !handle_div)
  {
    fputs(dlerror(), stderr);
    exit(1);
  }
  add_op = dlsym(handle_add, "add");
  sub_op = dlsym(handle_sub, "subtract");
  mul_op = dlsym(handle_mul, "multiply");
  div_op = dlsym(handle_div, "divide");
  if ((error = dlerror()) != NULL)
  {
    fprintf(stderr, "%s\n", error);
    exit(1);
  }

  op_struct operations[OPER_COUNT];
  operations[0].operation = add_op;
  operations[0].name = "Addition";
  operations[1].operation = sub_op;
  operations[1].name = "Subtraction";
  operations[2].operation = mul_op;
  operations[2].name = "Multiplication";
  operations[3].operation = div_op;
  operations[3].name = "Division";
  int command = 1;
  while (command > 0)
  {
    command = get_command_interface(operations);
    if (command > OPER_COUNT)
    {
      printf("Unknown command, try again\n");
      continue;
    }
    if (command > 0 && command <= OPER_COUNT)
      print_operation(operations[command - 1]);
  }

  dlclose(handle_add);
  dlclose(handle_sub);
  dlclose(handle_mul);
  dlclose(handle_div);

  return 0;
}