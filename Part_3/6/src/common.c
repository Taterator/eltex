#include "common.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

void send_msg(int qid, int msgtype, int sender, char* message) {
  struct msgbufl msg;

  msg.mtype = msgtype;
  msg.sender = sender;
  strcpy(msg.mtext, message);

  if (msgsnd(qid, &msg, sizeof(struct msgbufl) - sizeof(long), IPC_NOWAIT) ==
      -1) {
    perror("msgsnd error");
  }
}

int get_msg(int qid, int msgtype, struct msgbufl* msg) {
  if (msgrcv(qid, msg, sizeof(*msg) - sizeof(msg->mtype), msgtype,
             MSG_NOERROR | IPC_NOWAIT) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
    return 0;
  }
  return 1;
}

void print_msg(int qid, int msgtype) {
  struct msgbufl msg;

  if (msgrcv(qid, &msg, sizeof(msg) - sizeof(msg.mtype), msgtype,
             MSG_NOERROR | IPC_NOWAIT) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
  } else {
    printf("\nFrom: %d; message received: %s\n", msg.sender, msg.mtext);
  }
}