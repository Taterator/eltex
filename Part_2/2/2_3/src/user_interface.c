#include <stdio.h>
#include "calc.h"
#include <math.h>

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
  op_struct operations[OPER_COUNT];
  operations[0].operation = add;
  operations[0].name = "Addition";
  operations[1].operation = subtract;
  operations[1].name = "Subtraction";
  operations[2].operation = multiply;
  operations[2].name = "Multiplication";
  operations[3].operation = divide;
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
  return 0;
}