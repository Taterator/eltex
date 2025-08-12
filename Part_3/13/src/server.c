#define _DEFAULT_SOURCE
#include <arpa/inet.h>
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

// функция обслуживания
// подключившихся пользователей
void dostuff(int);

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

// количество активных пользователей
int nclients = 0;

// печать количества активных пользователей
void printusers() {
  if (nclients) {
    printf("%d user on-line\n", nclients);
  } else {
    printf("No User on line\n");
  }
}

void signal_handler(int signal) {
  switch (signal) {
    case SIGUSR1:
      nclients--;
      printusers();
      break;

    default:
      break;
  }
}

// функция обработки данных
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

int main(int argc, char *argv[]) {
  // char buff[1024];        // Буфер для различных нужд
  int sockfd, newsockfd;  // дескрипторы сокетов
  int portno;             // номер порта
  int pid;                // id номер потока
  socklen_t clilen;       // размер адреса клиента типа socklen_t
  struct sockaddr_in serv_addr, cli_addr;  // структура сокета сервера и клиента
  printf("TCP SERVER DEMO\n");
  // ошибка в случае если мы не указали порт
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  // Шаг 1 - создание сокета
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");
  // Шаг 2 - связывание сокета с локальным адресом
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr =
      INADDR_ANY;  // сервер принимает подключения на все IP - адреса
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  // Шаг 3 - ожидание подключений, размер очереди - 5
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на
  // подключение)
  while (1) {
    signal(SIGUSR1, signal_handler);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");
    nclients++;
    // вывод сведений о клиенте
    struct hostent *hst;
    hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
    printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "Unknown host",
           (char *)inet_ntoa(cli_addr.sin_addr));
    pid = fork();
    if (pid < 0) error("ERROR on fork");
    if (pid == 0) {
      printusers();
      close(sockfd);
      dostuff(newsockfd);
      kill(getppid(), SIGUSR1);
      exit(0);
    } else
      close(newsockfd);
  }
  close(sockfd);
  return 0;
}

void dostuff(int sock) {
  int bytes_recv;  // размер принятого сообщения
  int a, b, c;     // переменные для myfunc
  char buff[20 * 1024];
#define str0 \
  "Enter choice: 1 - add, 2 - sub, 3 - mul, 4 - div, 5 - download file"
#define str1 "Enter 1 parameter"
#define str2 "Enter 2 parameter"
#define str3 "Enter filename"

  // отправляем клиенту сообщение
  write(sock, str0, sizeof(str0));

  // обработка первого параметра
  bytes_recv = read(sock, &buff[0], sizeof(buff));
  if (bytes_recv < 0) error("ERROR reading from socket");
  c = atoi(buff);  // преобразование параметра выбора в int
  if (c == 5) {
    // отправляем клиенту сообщение
    write(sock, str3, sizeof(str3));
    // обработка первого параметра
    FILE *fp;
    bytes_recv = read(sock, &buff[0], sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    buff[bytes_recv - 1] = 0;
    fp = fopen(buff, "r");
    if (fp == NULL) {
    }
    send_file(fp, sock);
  } else {
    // отправляем клиенту сообщение
    write(sock, str1, sizeof(str1));

    // обработка первого параметра
    bytes_recv = read(sock, &buff[0], sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    a = atoi(buff);  // преобразование первого параметра в int
    // отправляем клиенту сообщение
    write(sock, str2, sizeof(str2));
    bytes_recv = read(sock, &buff[0], sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    b = atoi(buff);       // преобразование второго параметра в int
    a = myfunc(a, b, c);  // вызов пользовательской функции
    snprintf(buff, sizeof(buff), "%d",
             a);                // преобразование результата в строку
    buff[strlen(buff)] = '\0';  // добавление к сообщению символа конца строки
    // отправляем клиенту результат
    write(sock, &buff[0], sizeof(buff));
  }
  close(sock);
  printf("-disconnect\n");
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