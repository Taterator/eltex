#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "udp_msg.h"
#include <linux/if_packet.h>

#define END_MSG "exit"
#define MSG_BUF_SIZE 1500

int run;

static void sigint_call(int signal_code)
{
  switch (signal_code)
  {
  case SIGINT:
    run = 0;
    break;

  default:
    break;
  }
  return;
}

int main(int argc, char *argv[])
{
  // this ip, this port, other ip, other port
  if (argc < 5)
  {
    printf("Usage: ./client this_ip this_port other_ip other_port\n");
    exit(EXIT_SUCCESS);
  }
  char *self_ip = argv[1];
  int self_port = atoi(argv[2]);
  char *other_ip = argv[3];
  int other_port = atoi(argv[4]);
  pid_t pid;
  int rv;

  struct sigaction psa;
  psa.sa_handler = sigint_call;
  sigaction(SIGINT, &psa, NULL);

  int socket_desc;
  struct sockaddr_in self_addr, other_addr, sin_buf;
  char recieved_message[MSG_BUF_SIZE], sent_message[MSG_BUF_SIZE];
  memset(recieved_message, '\0', sizeof(recieved_message));
  memset(sent_message, '\0', sizeof(sent_message));

  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(self_port);
  if (inet_pton(AF_INET, self_ip, &self_addr.sin_addr) <= 0)
  {
    perror("Invalid self IP");
    exit(EXIT_FAILURE);
  }

  other_addr.sin_family = AF_INET;
  other_addr.sin_port = htons(other_port);
  if (inet_pton(AF_INET, other_ip, &other_addr.sin_addr) <= 0)
  {
    perror("Invalid other IP");
    exit(EXIT_FAILURE);
  }

  socket_desc = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);

  if (socket_desc == -1)
  {
    perror("Failed to create socket");
    exit(1);
  }

  int one = 1;
  const int *val = &one;
  if (setsockopt(socket_desc, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
  {
    perror("Error setting IP_HDRINCL");
    exit(0);
  }

  switch (pid = fork())
  {
  case -1:
    perror("Unable to fork");
    exit(EXIT_FAILURE);
  case 0:
    run = 1;
    while (run)
    {
      memset(recieved_message, '\0', sizeof(recieved_message));
      int ret = 0;
      if ((ret = recieve_udp(socket_desc, recieved_message, &sin_buf, self_addr)) < 0)
      {
        if (ret == -2)
          continue;
        if (errno != EAGAIN)
        {
          printf("Error while receiving msg\n");
        }
      }
      else
      {
        printf("You recieved: %s\nEnter message: ", recieved_message);
        fflush(stdout);
      }
    }
    close(socket_desc);
    _exit(EXIT_SUCCESS);

  default:
    run = 1;
    int exit_flag = 0;
    printf("Enter message: ");
    fflush(stdout);
    while (run)
    {
      memset(sent_message, '\0', sizeof(sent_message));

      fgets(sent_message, MSG_BUF_SIZE, stdin);
      sent_message[strcspn(sent_message, "\n")] = 0;
      if (strcmp(sent_message, END_MSG) == 0)
      {
        run = 0;
      }
      if (strlen(sent_message) == 0)
        continue;
      if (send_udp(socket_desc, sent_message, self_addr, other_addr) < 0)
      {
        printf("Unable to send message\n");
      }
    }
    if (!exit_flag)
    {
      send_udp(socket_desc, END_MSG, self_addr, other_addr);
    }
    kill(pid, SIGKILL);
    wait(&rv);
  }
  close(socket_desc);

  return 0;
}
