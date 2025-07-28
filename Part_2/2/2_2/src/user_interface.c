#include <stdio.h>
#include "calc.h"
#include <math.h>

void print_result(float res)
{
  printf("-> %.3f\n", res);
}

int get_command_interface()
{
  printf("| Enter command:\n| 1 - add, 2 - subtract, 3 - multiply, 4 - divide\n| 5 - add up to 5 numbers, quit - to exit\n");

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

int add_many_inteface(float *res)
{
  int count = 0;
  float numbers[5];
  printf("| Enter the amount of numbers:\n");
  scanf("%d", &count);
  if (count > 5 || count < 1)
  {
    printf("| Error: Wrong amount of numbers\n");
    return 1;
  }
  for (int i = 0; i < count; i++)
  {
    printf("| Enter number #%d:\n", i + 1);
    scanf("%f", &numbers[i]);
  }
  switch (count)
  {
  case 0:
    add_n(count, res);
    break;
  case 1:
    add_n(count, res, numbers[0]);
    break;
  case 2:
    add_n(count, res, numbers[0], numbers[1]);
    break;
  case 3:
    add_n(count, res, numbers[0], numbers[1], numbers[2]);
    break;
  case 4:
    add_n(count, res, numbers[0], numbers[1], numbers[2], numbers[3]);
    break;
  case 5:
    add_n(count, res, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
    break;

  default:
    break;
  }
  return 0;
}

int main()
{
  float num_1 = 0;
  float num_2 = 0;
  float res = 0;
  int command = 1;
  int error = 0;
  while (command > 0)
  {
    command = get_command_interface();

    switch (command)
    {
    case 1:
      get_numbers_inteface(&num_1, &num_2);
      error = add(num_1, num_2, &res);
      break;

    case 2:
      get_numbers_inteface(&num_1, &num_2);
      error = subtract(num_1, num_2, &res);
      break;

    case 3:
      get_numbers_inteface(&num_1, &num_2);
      error = multiply(num_1, num_2, &res);
      break;

    case 4:
      get_numbers_inteface(&num_1, &num_2);
      error = divide(num_1, num_2, &res);
      break;

    case 5:
      error = add_many_inteface(&res);
      break;

    default:
      break;
    }
    if (command > 0 && command < 6 && !error)
      print_result(res);
  }
  return 0;
}