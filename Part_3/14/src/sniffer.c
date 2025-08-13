#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PACKET_SIZE 65535
#define DUMP_FILE "dump.bin"
#define DUMP_FILE2 "dump2.bin"

static int sockfd = -1;
static FILE *dump_file = NULL;
static FILE *dump_file2 = NULL;

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

int main()
{
  signal(SIGINT, signal_handler);

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
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
  dump_file2 = fopen(DUMP_FILE2, "wb");
  if (!dump_file2)
  {
    perror("fopen");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);
  char buffer[MAX_PACKET_SIZE];
  while (1)
  {
    int packet_size = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addr_len);
    if (packet_size < 0)
    {
      perror("recvfrom");
      continue;
    }
    char dest[300];
    printf("Got packet from port: %d, ip: %s\n", ntohs(addr.sin_port), inet_ntop(AF_INET, &addr.sin_addr, dest, 300));
    fwrite(buffer, packet_size, 1, dump_file);
    fflush(dump_file);
    fwrite(buffer, 1, packet_size, dump_file2);
    fflush(dump_file2);
  }

  fclose(dump_file);
  close(sockfd);
  return 0;
}