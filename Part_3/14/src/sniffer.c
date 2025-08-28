#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <linux/if_packet.h>

#define MAX_PACKET_SIZE 65535
#define DUMP_FILE "dump.bin"

static int sockfd = -1;
static FILE *dump_file = NULL;
static int port = -1;

#define BUF_LEN 131072
static char parser_buffer[BUF_LEN];

void signal_handler(int signum)
{
  if (signum == SIGINT)
  {
    printf("Exiting\n");
    if (dump_file)
    {
      fclose(dump_file);
    }
    if (sockfd >= 0)
    {
      close(sockfd);
    }
    exit(EXIT_SUCCESS);
  }
}

void mac_to_str(uint8_t mac[ETH_ALEN], char *str)
{
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
          mac[0], mac[1], mac[2],
          mac[3], mac[4], mac[5]);
}

void parse_udp(char *buf, int buf_len)
{
  struct udphdr *udph = (struct udphdr *)(buf);
  if (!(ntohs(udph->source) == port || port == -1))
    return;

  sprintf(parser_buffer + strlen(parser_buffer), "\n<<< UDP HEADER >>>\n");
  sprintf(parser_buffer + strlen(parser_buffer), "Source Port: %d\n", ntohs(udph->source));
  sprintf(parser_buffer + strlen(parser_buffer), "Destination Port: %d\n", ntohs(udph->dest));
  sprintf(parser_buffer + strlen(parser_buffer), "Length: %d\n", ntohs(udph->len));
  sprintf(parser_buffer + strlen(parser_buffer), "Checksum: 0x%x\n", ntohs(udph->check));

  char *payload = buf + sizeof(struct udphdr);
  int payload_size = ntohs(udph->len) - sizeof(struct udphdr);

  sprintf(parser_buffer + strlen(parser_buffer), "\n<<< PAYLOAD (%d bytes) >>>\n", payload_size);
  for (int i = 0; i < payload_size; i++)
  {
    sprintf(parser_buffer + strlen(parser_buffer), "%c", payload[i]);
    if ((i + 1) % 8 == 0)
      sprintf(parser_buffer + strlen(parser_buffer), " ");
    if ((i + 1) % 16 == 0)
      sprintf(parser_buffer + strlen(parser_buffer), "\n");
  }
  sprintf(parser_buffer + strlen(parser_buffer), "\n*********************************************************\n");

  printf("%s\n", parser_buffer);
  fwrite(buf, buf_len, 1, dump_file);
  fflush(dump_file);
}

void parse_ip(char *buf, int buf_len)
{
  struct iphdr *iph = (struct iphdr *)(buf);
  sprintf(parser_buffer + strlen(parser_buffer), "\n<<< IP HEADER >>>\n");
  sprintf(parser_buffer + strlen(parser_buffer), "Version: %d\n", iph->version);
  sprintf(parser_buffer + strlen(parser_buffer), "Header Length: %d bytes (%d)\n", iph->ihl * 4, iph->ihl);
  sprintf(parser_buffer + strlen(parser_buffer), "DS Field: %d\n", iph->tos);
  sprintf(parser_buffer + strlen(parser_buffer), "Total Length: %d\n", ntohs(iph->tot_len));
  sprintf(parser_buffer + strlen(parser_buffer), "Identification: 0x%x (%d)\n", ntohs(iph->id), ntohs(iph->id));
  sprintf(parser_buffer + strlen(parser_buffer), "Flags: 0x%x\n", iph->frag_off);
  uint16_t mask = 0b0001111111111111; // Mask to get fragmentation offset
  sprintf(parser_buffer + strlen(parser_buffer), "Fragment offset: %d\n", ntohs(iph->frag_off) & mask);

  sprintf(parser_buffer + strlen(parser_buffer), "TTL: %d\n", iph->ttl);
  sprintf(parser_buffer + strlen(parser_buffer), "Protocol: %d\n", iph->protocol);
  sprintf(parser_buffer + strlen(parser_buffer), "Checksum: 0x%x\n", ntohs(iph->check));

  struct in_addr src_addr, dst_addr;
  src_addr.s_addr = iph->saddr;
  dst_addr.s_addr = iph->daddr;
  sprintf(parser_buffer + strlen(parser_buffer), "Source IP: %s\n", inet_ntoa(src_addr));
  sprintf(parser_buffer + strlen(parser_buffer), "Destination IP: %s\n", inet_ntoa(dst_addr));

  switch (iph->protocol)
  {
  case IPPROTO_UDP:
    parse_udp(buf + sizeof(struct iphdr), buf_len - sizeof(struct iphdr));
    return;
  case IPPROTO_TCP:
    return;
  case IPPROTO_ICMP:
    return;
  default:
    // printf("Unknown protocol\n");
    return;
  }
}

void parse_ethernet(char *buf, int buf_len)
{
  struct ether_header *ethhdr = (struct ether_header *)buf;
  char mac[32];
  sprintf(parser_buffer + strlen(parser_buffer), "<<< ETHERNET HEADER >>>\n");
  mac_to_str(ethhdr->ether_dhost, mac);
  sprintf(parser_buffer + strlen(parser_buffer), "Destination MAC: %s\n", mac);
  mac_to_str(ethhdr->ether_shost, mac);
  sprintf(parser_buffer + strlen(parser_buffer), "Source MAC: %s\n", mac);
  sprintf(parser_buffer + strlen(parser_buffer), "Type: 0x%04X\n", ntohs(ethhdr->ether_type));

  switch (ntohs(ethhdr->ether_type))
  {
  case ETHERTYPE_IP:
    parse_ip(buf + sizeof(struct ether_header), buf_len - sizeof(struct ether_header));
    return;
    break;

  default:
    // printf("Unknown ethernet type: %d\n", ntohs(ethhdr->ether_type));
    return;
    break;
  }
}

void packet_parser(char *buf, int buf_len)
{
  memset(parser_buffer, 0, BUF_LEN);
  parse_ethernet(buf, buf_len);
}

int main(int argc, char *argv[])
{
  if (argc > 1)
    port = atoi(argv[1]);
  signal(SIGINT, signal_handler);
  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockfd < 0)
  {
    perror("Error while creating socket");
    exit(EXIT_FAILURE);
  }

  dump_file = fopen(DUMP_FILE, "wb");
  if (!dump_file)
  {
    perror("fopen");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);
  char buffer[MAX_PACKET_SIZE];

  int val = 1;
  if (setsockopt(sockfd, SOL_PACKET, PACKET_IGNORE_OUTGOING, &val, sizeof(val)) < 0)
    perror("setsockopt");

  while (1)
  {
    int packet_size = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addr_len);
    if (packet_size < 0)
    {
      perror("recvfrom");
      continue;
    }
    packet_parser(buffer, packet_size);
  }

  fclose(dump_file);
  close(sockfd);
  return 0;
}