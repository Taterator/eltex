#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "udp_msg.h"

#define MAX_PACKET_SIZE 1500

unsigned short csum(unsigned short *ptr, int nbytes)
{
  register long sum;
  unsigned short oddbyte;
  register short answer;

  sum = 0;
  while (nbytes > 1)
  {
    sum += *ptr++;
    nbytes -= 2;
  }
  if (nbytes == 1)
  {
    oddbyte = 0;
    *((uint8_t *)&oddbyte) = *(uint8_t *)ptr;
    sum += oddbyte;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum = sum + (sum >> 16);
  answer = (short)~sum;

  return (answer);
}

int send_udp(int sock_fd, char *msg, const struct sockaddr_in src, const struct sockaddr_in dst)
{
  char datagram[MAX_PACKET_SIZE], *data;

  memset(datagram, 0, MAX_PACKET_SIZE);

  struct iphdr *iph = (struct iphdr *)datagram;
  struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct iphdr));
  struct sockaddr_in sin;

  data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
  snprintf(data, MAX_PACKET_SIZE - sizeof(struct iphdr) - sizeof(struct udphdr) - 1, "%s", msg);

  sin = dst;

  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
  iph->id = htonl(54321);
  iph->frag_off = 0x40;
  iph->ttl = 64;
  iph->protocol = IPPROTO_UDP;
  iph->check = 0;
  iph->saddr = src.sin_addr.s_addr;
  iph->daddr = sin.sin_addr.s_addr;

  iph->check = csum((unsigned short *)datagram, iph->tot_len);

  udph->source = src.sin_port;
  udph->dest = dst.sin_port;
  udph->len = htons(sizeof(struct udphdr) + strlen(msg));

  if (sendto(sock_fd, datagram, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    perror("sendto failed");
    return -1;
  }
  return 0;
}

int recieve_udp(int sock_fd, char *msg, struct sockaddr_in *src, const struct sockaddr_in dst)
{
  char buffer[MAX_PACKET_SIZE];
  memset(buffer, 0, MAX_PACKET_SIZE);
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  int packet_size = recvfrom(sock_fd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addr_len);
  if (packet_size < 0)
  {
    perror("recvfrom");
    return -1;
  }
  *src = addr;
  struct iphdr *iph = (struct iphdr *)(buffer);

  if (iph->protocol != IPPROTO_UDP)
  {
    return -2;
  }

  struct udphdr *udph = (struct udphdr *)(buffer + sizeof(struct iphdr));

  if (udph->dest != dst.sin_port || iph->daddr != dst.sin_addr.s_addr)
  {
    return -2;
  }
  src->sin_port = udph->uh_sport;

  char *payload = buffer + sizeof(struct udphdr) + sizeof(struct iphdr);
  int payload_size = ntohs(udph->len) - sizeof(struct udphdr);
  strncpy(msg, payload, payload_size);
  return 0;
}