#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void write_file(int sock);

void error(const char *msg) {
  perror(msg);
  exit(0);
}
int main(int argc, char *argv[]) {
  int my_sock, portno, n;
  struct sockaddr_in serv_addr;
  // struct hostent *server;
  char buff[1024];
  printf("TCP DEMO CLIENT\n");
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(EXIT_SUCCESS);
  }
  // извлечение порта
  portno = atoi(argv[2]);
  // Шаг 1 - создание сокета
  my_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (my_sock < 0) error("ERROR opening socket");
  // заполнение структуры serv_addr
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  // установка порта
  serv_addr.sin_port = htons(portno);
  // Шаг 2 - установка соединения
  if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  int dl_flag = 0;
  // Шаг 3 - чтение и передача сообщений
  while ((n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) > 0) {
    // ставим завершающий ноль в конце строки
    buff[n] = 0;
    // выводим на экран
    printf("S=>C:%s\n", buff);
    // читаем пользовательский ввод с клавиатуры
    printf("S<=C:");
    fgets(&buff[0], sizeof(buff) - 1, stdin);
    // проверка на "quit"
    if (!strcmp(&buff[0], "quit\n")) {
      // Корректный выход
      printf("Exit...\n");
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    // передаем строку клиента серверу
    if (send(my_sock, &buff[0], strlen(&buff[0]), 0) < 0) {
      // printf("END \n");
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    if (dl_flag) {
      write_file(my_sock);
      close(my_sock);
      exit(EXIT_SUCCESS);
    }
    if (!strcmp(&buff[0], "5\n")) {
      dl_flag = 1;
    }
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
  char buffer[1024];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sock, buffer, 1024, 0);
    if (n <= 0) {
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }
  return;
}