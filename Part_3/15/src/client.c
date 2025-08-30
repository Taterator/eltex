#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PORT 55555
#define DEFAULT_IP "127.0.0.1"
#define BUF_SIZE 1024

static int my_sock;

void write_file(int sock);

void signal_handler(int signal) {
  if (signal == SIGPIPE) {
    close(my_sock);
    exit(EXIT_SUCCESS);
  }
}

void error(const char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  signal(SIGPIPE, signal_handler);
  int portno, n;
  int first_param = 1;
  struct sockaddr_in serv_addr;
  char buff[BUF_SIZE];
  printf("TCP DEMO CLIENT\n");

  my_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (my_sock < 0) error("ERROR opening socket");
  bzero((char *)&serv_addr, sizeof(serv_addr));
  if (argc < 3) {
    printf("Starting with default port: %d, default ip: %s\n", DEFAULT_PORT,
           DEFAULT_IP);
    portno = DEFAULT_PORT;
    serv_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
  } else {
    portno = atoi(argv[2]);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  int dl_flag = 0;
  while ((n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) > 0) {
    buff[n] = 0;
    printf("S=>C:%s\n", buff);
    printf("S<=C:");
    fgets(&buff[0], sizeof(buff) - 1, stdin);
    if (!strcmp(&buff[0], "quit\n")) {
      printf("Exit...\n");
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    if (send(my_sock, &buff[0], strlen(&buff[0]), 0) < 0) {
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    if (dl_flag) {
      write_file(my_sock);
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    if (!strcmp(&buff[0], "5\n") && first_param) {
      dl_flag = 1;
    }
    first_param = 0;
  }
  printf("Recv error \n");
  fflush(stdout);
  close(my_sock);
  exit(EXIT_FAILURE);
}

void write_file(int sock) {
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[BUF_SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sock, buffer, BUF_SIZE, 0);
    if (n <= 0) {
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, BUF_SIZE);
  }
  return;
}