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

#define MSG_BUF_SIZE 1500
#define CLIENTS_COUNT 10
#define END_MSG "exit"

typedef struct
{
  struct sockaddr_in address;
  int count;
} client_t;

typedef struct
{
  client_t *clients[CLIENTS_COUNT];
  unsigned int client_count;
} client_list;

void print_total_client_count();
client_t *new_client(struct sockaddr_in address);
void free_client(client_t *client);
int equals_addr(const struct sockaddr_in address_1, const struct sockaddr_in address_2);
void init_clients();
void free_clients();
client_t *add_client(client_t *client);
client_t *find_client(const struct sockaddr_in address);
int remove_client(client_t *client);
int client_get_count(client_t *client);
void client_count_add(client_t *client);

static int run;
static int socket_desc;

static client_list cl_list;

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
  pid_t pid;
  int rv;
  char *self_ip;
  int self_port;

  if (argc < 3)
  {
    self_ip = "127.0.0.23";
    self_port = 55555;
    printf("TEST WITH IP: %s, PORT: %d\n", self_ip, self_port);
  }
  else
  {
    self_ip = argv[1];
    self_port = atoi(argv[2]);
  }

  struct sigaction psa;
  psa.sa_handler = sigint_call;
  sigaction(SIGINT, &psa, NULL);

  init_clients();

  struct sockaddr_in self_addr, other_addr, sin_buf;
  char recieved_message[MSG_BUF_SIZE], sent_message[MSG_BUF_SIZE];

  memset(recieved_message, '\0', sizeof(recieved_message));
  memset(sent_message, '\0', sizeof(sent_message));

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

  self_addr.sin_family = AF_INET;
  self_addr.sin_port = htons(self_port);
  if (inet_pton(AF_INET, self_ip, &self_addr.sin_addr) <= 0)
  {
    perror("Invalid self IP");
    close(socket_desc);
    exit(EXIT_FAILURE);
  }

  run = 1;

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
        other_addr = sin_buf;
        client_t *client = find_client(other_addr);
        if (client == NULL)
        {
          client = add_client(new_client(other_addr));
        }
        char dest[300];
        printf("Got packet from port: %d, ip: %s\n", ntohs(other_addr.sin_port), inet_ntop(AF_INET, &other_addr.sin_addr, dest, 300));
        printf("You recieved: %s\n", recieved_message);
        fflush(stdout);
        if (strcmp(recieved_message, END_MSG) == 0)
        {
          remove_client(client);
        }
        else
        {
          client_count_add(client);
          sprintf(recieved_message + strlen(recieved_message), " %d", client_get_count(client));
          if (send_udp(socket_desc, recieved_message, self_addr, other_addr) < 0)
          {
            printf("Unable to send message\n");
          }
        }
      }
    }
    free_clients();
    close(socket_desc);
    _exit(EXIT_SUCCESS);

  default:
    run = 1;
    while (run)
    {
      sleep(1);
    }

    kill(pid, SIGKILL);
    wait(&rv);
  }
  free_clients();
  close(socket_desc);

  return 0;
}

void print_total_client_count()
{
  printf("Total clients: %d\n", cl_list.client_count);
}

client_t *new_client(struct sockaddr_in address)
{
  client_t *cl = malloc(sizeof(client_t));
  cl->address = address;
  cl->count = 0;
  char dest[300];
  printf("NEW CLIENT: %d, ip: %s\n",
         ntohs(cl->address.sin_port), inet_ntop(AF_INET, &cl->address.sin_addr, dest, 300));
  return cl;
};

void free_client(client_t *client)
{
  free(client);
}

int equals_addr(const struct sockaddr_in address_1, const struct sockaddr_in address_2)
{
  const int buf_size = 50;
  char ip_1[buf_size];
  char ip_2[buf_size];
  uint16_t port_1 = ntohs(address_1.sin_port);
  uint16_t port_2 = ntohs(address_2.sin_port);
  inet_ntop(AF_INET, &address_1.sin_addr, ip_1, buf_size);
  inet_ntop(AF_INET, &address_2.sin_addr, ip_2, buf_size);

  return ((strcmp(ip_1, ip_2) == 0) && (port_1 == port_2));
}

void init_clients()
{
  for (int i = 0; i < CLIENTS_COUNT; i++)
  {
    cl_list.clients[i] = NULL;
  }
  cl_list.client_count = 0;
}

void free_clients()
{
  for (unsigned int i = 0; i < cl_list.client_count; i++)
  {
    if (cl_list.clients[i] != NULL)
      free(cl_list.clients[i]);
  }
}

client_t *add_client(client_t *client)
{
  if (cl_list.client_count >= CLIENTS_COUNT)
  {
    return NULL;
  }
  cl_list.clients[cl_list.client_count++] = client;
  print_total_client_count();
  return client;
}

client_t *find_client(const struct sockaddr_in address)
{
  for (unsigned int i = 0; i < cl_list.client_count; i++)
  {
    if (equals_addr(cl_list.clients[i]->address, address))
    {
      return cl_list.clients[i];
    }
  }
  return NULL;
}

int remove_client(client_t *client)
{
  for (unsigned int i = 0; i < cl_list.client_count; i++)
  {
    if (cl_list.clients[i] == client)
    {
      free_client(client);
      cl_list.clients[i] = cl_list.clients[cl_list.client_count - 1];
      cl_list.clients[cl_list.client_count - 1] = NULL;
      cl_list.client_count--;
      print_total_client_count();
      return 0;
    }
  }
  return 1;
}

int client_get_count(client_t *client)
{
  return client->count;
}

void client_count_add(client_t *client)
{
  client->count++;
}
