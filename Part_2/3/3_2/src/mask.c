#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mask.h"

int parse_ip(char *str, ip_t *ip)
{
  for (int i = 0; i < IP_FIELDS; i++)
  {
    ip->values[0] = 0;
  }
  char delimiter[] = ".";
  char *token = strtok(str, delimiter);

  for (int i = 0; i < IP_FIELDS; i++)
  {
    ip->values[i] = atoi(token);
    token = strtok(NULL, delimiter);
  }

  return 0;
}

int apply_mask(const ip_t ip, const ip_t mask, ip_t *result)
{
  for (int i = 0; i < IP_FIELDS; i++)
  {
    result->values[i] = ip.values[i] & mask.values[i];
  }
  return 0;
}

ip_t random_ip()
{
  ip_t result;
  for (int i = 0; i < IP_FIELDS; i++)
  {
    result.values[i] = rand() % 256;
  }
  return result;
}

int ip_is_equal(const ip_t ip_1, const ip_t ip_2)
{
  for (int i = 0; i < IP_FIELDS; i++)
  {
    if (ip_1.values[i] != ip_2.values[i])
      return 0;
  }
  return 1;
}

ip_t ip_and(const ip_t ip_1, const ip_t ip_2)
{
  ip_t res;
  for (int i = 0; i < IP_FIELDS; i++)
  {
    res.values[i] = ip_1.values[i] & ip_2.values[i];
  }
  return res;
}

int in_subnet(const ip_t ip, const ip_t subnet_ip, const ip_t mask_ip)
{
  return (ip_is_equal(subnet_ip, ip_and(ip, mask_ip)));
}

int count_valid_packets(const ip_t subnet_ip, const ip_t mask_ip, const int packet_count)
{
  srand(time(NULL));
  int counter = 0;
  for (int i = 0; i < packet_count; i++)
  {
    ip_t r_ip = random_ip();
    if (in_subnet(r_ip, subnet_ip, mask_ip))
      counter++;
  }
  return counter;
}