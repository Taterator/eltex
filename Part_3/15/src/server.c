#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_EVENTS 5
#define MAX_CLIENTS 5
#define DEFAULT_PORT 55555

struct client_list {
  int count;
  int fd[MAX_CLIENTS];
  int state[MAX_CLIENTS];
  int operation[MAX_CLIENTS];
  int buffer[MAX_CLIENTS];
};

// функции обслуживания
// подключившихся пользователей
void send_start(int);
void handle_client(int);

int add(int, int);
int sub(int, int);
int mul(int, int);
int divide(int, int);
void send_file(FILE *fp, int sock);

// функция обработки ошибок
void error(const char *msg) {
  perror(msg);
  exit(1);
}

static struct client_list cl_list;

// STATES
// -1 - uninit, 0 - sending choice
// 1 - first arg, 2 - second arg
#define ST_UNINIT -1
#define ST_CHOICE 0
#define ST_FIRST 1
#define ST_SECOND 2

// OPERATIONS
#define OP_UNINIT -1
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_FILE 5

// Работа со списком клиентов

void init_clients();
void add_client(int cl_fd);
void remove_client(int cl_fd);
int get_client_index(int cl_fd);
int get_client_state(int cl_fd);
void set_client_state(int cl_fd, int state);
int get_client_operation(int cl_fd);
void set_client_operation(int cl_fd, int operation);
int get_client_buffer(int cl_fd);
void set_client_buffer(int cl_fd, int buffer_val);

// печать количества активных пользователей
void printusers();

void signal_handler(int signal);

void disconnect_client(int sock);

// функция обработки данных
int myfunc(int a, int b, int c);

int main(int argc, char *argv[]) {
  init_clients();
  int sockfd, newsockfd;  // дескрипторы сокетов
  int portno;             // номер порта
  socklen_t clilen;       // размер адреса клиента типа socklen_t
  struct sockaddr_in serv_addr, cli_addr;  // структура сокета сервера и клиента
  printf("TCP SERVER DEMO\n");
  if (argc < 2) {
    printf("Starting with default port: %d\n", DEFAULT_PORT);
    portno = DEFAULT_PORT;
  } else {
    portno = atoi(argv[1]);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  listen(sockfd, MAX_CLIENTS);
  clilen = sizeof(cli_addr);

  struct epoll_event ev, events[MAX_EVENTS];
  int epollfd = epoll_create1(0);
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);

  while (1) {
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == sockfd) {
        printf("New connection\n");
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        add_client(newsockfd);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = newsockfd;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, newsockfd, &ev);
        struct hostent *hst;
        hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
        printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "Unknown host",
               (char *)inet_ntoa(cli_addr.sin_addr));
        printusers();
        send_start(newsockfd);
      } else {
        handle_client(events[n].data.fd);
      }
    }
  }
  close(sockfd);
  close(epollfd);
  return 0;
}

void init_clients() {
  cl_list.count = 0;
  for (int i = 0; i < MAX_CLIENTS; i++) {
    cl_list.fd[i] = -1;
    cl_list.state[i] = ST_UNINIT;
    cl_list.operation[i] = OP_UNINIT;
    cl_list.buffer[i] = 0;
  }
}

void add_client(int cl_fd) {
  int flag = 0;
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.state[i] == ST_UNINIT) {
      cl_list.fd[i] = cl_fd;
      cl_list.state[i] = ST_CHOICE;
      flag = 1;
      break;
    }
  }
  if (flag)
    cl_list.count++;
  else
    error("Unable to add client");
}

void remove_client(int cl_fd) {
  int flag = 0;
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      cl_list.state[i] = ST_UNINIT;
      cl_list.fd[i] = -1;
      cl_list.operation[i] = ST_UNINIT;
      cl_list.buffer[i] = 0;
      flag = 1;
      break;
    }
  }
  if (flag)
    cl_list.count--;
  else
    error("Unable to remove client");
}

int get_client_index(int cl_fd) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      return i;
    }
  }
  return -1;
}

int get_client_state(int cl_fd) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      return cl_list.state[i];
    }
  }
  return -1;
}

void set_client_state(int cl_fd, int state) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      cl_list.state[i] = state;
      return;
    }
  }
}

int get_client_operation(int cl_fd) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      return cl_list.operation[i];
    }
  }
  return -1;
}

void set_client_operation(int cl_fd, int operation) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      cl_list.operation[i] = operation;
      return;
    }
  }
}

int get_client_buffer(int cl_fd) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      return cl_list.buffer[i];
    }
  }
  return -1;
}

void set_client_buffer(int cl_fd, int buffer_val) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (cl_list.fd[i] == cl_fd) {
      cl_list.buffer[i] = buffer_val;
      return;
    }
  }
}

void printusers() {
  if (cl_list.count) {
    printf("%d user on-line\n", cl_list.count);
  } else {
    printf("No User on line\n");
  }
}

void signal_handler(int signal) {
  switch (signal) {
    case SIGUSR1:
      cl_list.count--;
      printusers();
      break;

    default:
      break;
  }
}

void disconnect_client(int sock) {
  close(sock);
  remove_client(sock);
  printf("-disconnect\n");
  printusers();
}

int myfunc(int a, int b, int c) {
  switch (c) {
    case 1:
      return add(a, b);
    case 2:
      return sub(a, b);
    case 3:
      return mul(a, b);
    case 4:
      return divide(a, b);
    default:
      return 0;
  }
  return 0;
}

void send_start(int sock) {
#define str0 \
  "Enter choice: 1 - add, 2 - sub, 3 - mul, 4 - div, 5 - download file"
  // отправляем клиенту сообщение
  write(sock, str0, sizeof(str0));
}

void handle_client(int sock) {
  int bytes_recv;  // размер принятого сообщения
  int a, b, c;     // переменные для myfunc
  char buff[20 * 1024];
#define str1 "Enter 1 parameter"
#define str2 "Enter 2 parameter"
#define str3 "Enter filename"

  switch (get_client_operation(sock)) {
    case OP_UNINIT:
      bytes_recv = read(sock, &buff[0], sizeof(buff));
      if (bytes_recv < 0) error("ERROR reading from socket");
      c = atoi(buff);  // преобразование параметра выбора в int
      if (c > 5 || c < 1) {
        disconnect_client(sock);
        return;
      }
      if (c == 5) {
        // отправляем клиенту сообщение
        write(sock, str3, sizeof(str3));
        set_client_state(sock, ST_FIRST);
        set_client_operation(sock, OP_FILE);
      } else {
        // отправляем клиенту сообщение
        write(sock, str1, sizeof(str1));
        set_client_state(sock, ST_FIRST);
        set_client_operation(sock, c);
      }
      return;
      break;

    case OP_FILE:
      FILE *fp;
      bytes_recv = read(sock, &buff[0], sizeof(buff));
      if (bytes_recv < 0) error("ERROR reading from socket");
      buff[bytes_recv - 1] = 0;
      fp = fopen(buff, "r");
      if (fp == NULL) {
      }
      send_file(fp, sock);
      disconnect_client(sock);
      return;
      break;

    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
      switch (get_client_state(sock)) {
        case ST_FIRST:
          // обработка первого параметра
          bytes_recv = read(sock, &buff[0], sizeof(buff));
          if (bytes_recv < 0) error("ERROR reading from socket");
          set_client_buffer(sock, atoi(buff));
          set_client_state(sock, ST_SECOND);
          write(sock, str2, sizeof(str2));
          return;
          break;

        case ST_SECOND:
          bytes_recv = read(sock, &buff[0], sizeof(buff));
          if (bytes_recv < 0) error("ERROR reading from socket");
          b = atoi(buff);  // преобразование второго параметра в int
          a = myfunc(
              get_client_buffer(sock), b,
              get_client_operation(sock));  // вызов пользовательской функции
          snprintf(buff, sizeof(buff), "%d",
                   a);  // преобразование результата в строку
          buff[strlen(buff)] =
              '\0';  // добавление к сообщению символа конца строки
          // отправляем клиенту результат
          write(sock, &buff[0], sizeof(buff));
          disconnect_client(sock);
          return;

        default:
          return;
          break;
      }

    default:
      return;
      break;
  }
  return;
}

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return a / b; }
void send_file(FILE *fp, int sock) {
  char data[20 * 1024] = {0};

  while (fgets(data, sizeof(data), fp) != NULL) {
    if (send(sock, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, sizeof(data));
  }
}