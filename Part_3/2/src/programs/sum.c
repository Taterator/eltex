#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 10

int main()
{
  int sum = 0;
  int num = 0;
  char buffer[MAX_CHARS + 1];
  int count = 0;
  printf("Enter amount of numbers:\n");
  fgets(buffer, MAX_CHARS, stdin);
  count = atoi(buffer);
  printf("Input numbers:\n");
  for (int i = 0; i < count; i++)
  {
    fgets(buffer, MAX_CHARS, stdin);
    num = atoi(buffer);
    sum += num;
  }
  printf("Sum = %d\n", sum);

  return 0;
}