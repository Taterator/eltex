#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

void connect(int msqid, int id) {
  char *message = "CONNECT_CLIENT";
  send_msg(msqid, SERVER_ID, id, message);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    exit(EXIT_FAILURE);
  }
  int client_id = atoi(argv[1]);
  int msqid;
  msqid = msgget(KEY, 0666);
  if (msqid == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }
  // connect(msqid, client_id);
  int fd[2];
  if (pipe(fd) == -1) {
    return 1;
  }
  pid_t pid;
  char message[MAXTEXTSIZE];
  int run = 1;

  if ((pid = fork()) == 0) {  // Child process
    int flags = fcntl(fd[0], F_GETFL, 0);
    fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);
    close(fd[1]);
    int buf = 0;
    while (run) {
      print_msg(msqid, client_id);
      if (read(fd[0], &buf, sizeof(buf)) > 0) {
        run = 0;
        break;
      }
      usleep(100 * 1000);
    }
    close(fd[0]);
    _exit(EXIT_SUCCESS);
  }

  close(fd[0]);
  int num = 1;
  while (run) {  // Parent process
    printf("Enter message: ");
    scanf("%s", message);
    send_msg(msqid, SERVER_ID, client_id, message);
    if (strcmp(message, "shutdown") == 0) {
      run = 0;
      write(fd[1], &num, sizeof(num));
      close(fd[1]);
    }
  }

  wait(NULL);
  exit(EXIT_SUCCESS);
}