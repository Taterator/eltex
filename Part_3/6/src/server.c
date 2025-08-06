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

#define MAX_CLIENTS 10

static int clients[MAX_CLIENTS] = {0};

int add_client(int client_id) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] == 0) {
      clients[i] = client_id;
      return 0;
    }
  }
  printf("No client space\n");
  return 1;
}

int check_client(int client_id) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] == client_id) return 1;
  }
  return 0;
}

void delete_client(int client_id) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] == client_id) clients[i] = 0;
  }
}

void send_all(int msqid, struct msgbufl msg) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] != 0) {
      send_msg(msqid, clients[i], SERVER_ID, msg.mtext);
    }
  }
}

int main() {
  int msqid;
  msqid = msgget(KEY, IPC_CREAT | 0666);
  if (msqid == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }
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
    struct msgbufl msg_buf;
    while (run) {
      if (get_msg(msqid, SERVER_ID, &msg_buf)) {
        if (!check_client(msg_buf.sender)) {
          add_client(msg_buf.sender);
        }
        if (strcmp(msg_buf.mtext, "shutdown") == 0) {
          delete_client(msg_buf.sender);
        }
        printf("Got from %d: %s\n", msg_buf.sender, msg_buf.mtext);
        send_all(msqid, msg_buf);
      }
      if (read(fd[0], &buf, sizeof(buf)) > 0) {
        run = 0;
        break;
      }
      usleep(10 * 1000);
    }
    close(fd[0]);
    _exit(EXIT_SUCCESS);
  }

  close(fd[0]);
  int num = 1;
  while (run) {  // Parent process
    printf("***Type exit to exit***\n");
    scanf("%s", message);
    if (strcmp(message, "exit") == 0) {
      run = 0;
      write(fd[1], &num, sizeof(num));
      close(fd[1]);
    }
  }

  wait(NULL);
  msgctl(msqid, IPC_RMID, NULL);
  exit(EXIT_SUCCESS);
}