#ifndef COMMON_H_
#define COMMON_H_

#define KEY 46311
#define MAXTEXTSIZE 100
#define SERVER_ID 10

struct msgbufl {
  long mtype;
  int sender;
  char mtext[MAXTEXTSIZE];
};

void send_msg(int qid, int msgtype, int sender, char* message);
int get_msg(int qid, int msgtype, struct msgbufl* msg);
void print_msg(int qid, int msgtype);

#endif