#ifndef MASK_H_
#define MASK_H_

#include <stdint.h>

#define IP_FIELDS 4

typedef struct
{
  uint8_t values[IP_FIELDS];
} ip_t;

int parse_ip(char *str, ip_t *ip);
int apply_mask(const ip_t ip, const ip_t mask, ip_t *result);
ip_t random_ip();
int count_valid_packets(const ip_t subnet_ip, const ip_t mask_ip, const int packet_count);

#endif