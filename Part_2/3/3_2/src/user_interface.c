#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mask.h"

void print_ip(const ip_t ip)
{
  printf("%d", ip.values[0]);
  for (int i = 1; i < IP_FIELDS; i++)
  {
    printf(".%d", ip.values[i]);
  }
  printf("\n");
}

void print_args(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    printf("i = %d, %s \n", i, argv[i]);
  }
}

int main(int argc, char *argv[])
{
  ip_t ip, mask, subnet_ip;
  int packet_count = atoi(argv[3]);

  if (argc == 4)
  {
    parse_ip(argv[1], &ip);
    parse_ip(argv[2], &mask);
    apply_mask(ip, mask, &subnet_ip);
    int valid_packets = count_valid_packets(subnet_ip, mask, packet_count);
    int invalid_packets = packet_count - valid_packets;
    printf("Valid packets = %d (%.3f%%)\n", valid_packets, (float)valid_packets / packet_count * 100);
    printf("Invalid packets = %d (%.3f%%)\n", invalid_packets, (float)invalid_packets / packet_count * 100);
  }
  else
  {
    printf("Error: invalid number of arguments\n");
  }

  return 0;
}