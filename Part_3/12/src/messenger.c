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
    printf("Nope\n");
    exit(EXIT_SUCCESS);
  }
  char *self_ip = argv[1];
  int self_port = atoi(argv[2]);
  char *other_ip = argv[3];
  int other_port = atoi(argv[4]);
  pid_t pid;
  int rv;

  int socket_desc;
  struct sockaddr_in self_addr, other_addr;
  char recieved_message[2000], sent_message[2000];
  // unsigned int self_struct_length = sizeof(self_addr);
  unsigned int other_struct_length = sizeof(other_addr);

  // Clean buffers:
  memset(recieved_message, '\0', sizeof(recieved_message));
  memset(sent_message, '\0', sizeof(sent_message));

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (socket_desc < 0)
  {
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set port and IP:
  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(self_port);
  if (inet_pton(AF_INET, self_ip, &self_addr.sin_addr) <= 0)
  {
    perror("Invalid self IP");
    close(socket_desc);
    exit(EXIT_FAILURE);
  }

  other_addr.sin_family = AF_INET;
  other_addr.sin_port = htons(other_port);
  if (inet_pton(AF_INET, other_ip, &other_addr.sin_addr) <= 0)
  {
    perror("Invalid other IP");
    close(socket_desc);
    exit(EXIT_FAILURE);
  }

  if (bind(socket_desc, (struct sockaddr *)&self_addr, sizeof(self_addr)) < 0)
  {
    perror("bind");
    close(socket_desc);
    exit(EXIT_FAILURE);
  }

  switch (pid = fork())
  {
  case -1:
    perror("Unable to fork");
    exit(EXIT_FAILURE);
  case 0:
    run = 1;
    signal(SIGINT, sigint_call);
    while (run)
    {
      memset(recieved_message, '\0', sizeof(recieved_message));
      if (recvfrom(socket_desc, recieved_message, sizeof(recieved_message), 0,
                   (struct sockaddr *)&other_addr, &other_struct_length) < 0)
      {
        if (errno != EAGAIN)
          printf("Error while receiving msg\n");
        return -1;
      }
      else
      {
        printf("\nYou recieved: %s\nEnter message: ", recieved_message);
        fflush(stdout);
      }
    }
    _exit(EXIT_SUCCESS);

  default:
    run = 1;
    signal(SIGINT, sigint_call);
    while (run)
    {
      memset(sent_message, '\0', sizeof(sent_message));
      printf("Enter message: ");
      fflush(stdout);
      fgets(sent_message, 2000, stdin);
      sent_message[strcspn(sent_message, "\n")] = 0;
      if (strcmp(sent_message, "exit") == 0)
      {
        run = 0;
        break;
      }
      if (sendto(socket_desc, sent_message, strlen(sent_message), 0,
                 (struct sockaddr *)&other_addr, other_struct_length) < 0)
      {
        printf("Unable to send message\n");
        return -1;
      }
    }

    kill(pid, SIGKILL);
    wait(&rv);
  }
  close(socket_desc);

  return 0;
}
