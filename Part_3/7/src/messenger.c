#include "messenger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

void sigint_call(int signal_code)
{
  switch (signal_code)
  {
  case SIGINT:
    mq_unlink(QUEUE_NAME_1);
    mq_unlink(QUEUE_NAME_2);
    exit(0);

  default:
    printf("Error\n");
    break;
  }
}

int get_msg(char *buffer)
{
  printf("-> ");
  fgets(buffer, SIZE, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  return 0;
}

void init_queue_attr(struct mq_attr *q_attr)
{
  q_attr->mq_maxmsg = MAX_MSG;
  q_attr->mq_msgsize = SIZE;
  q_attr->mq_flags = 0;
  q_attr->mq_curmsgs = 0;
}

int ping()
{
  signal(SIGINT, sigint_call);
  mqd_t ds_1, ds_2;
  struct mq_attr queue_attr_1, queue_attr_2;

  init_queue_attr(&queue_attr_1);
  init_queue_attr(&queue_attr_2);
  if ((ds_1 = mq_open(QUEUE_NAME_1,
                      O_CREAT | O_WRONLY, 0600,
                      &queue_attr_1)) == (mqd_t)-1)
  {
    perror("Creating queue error");
    return -1;
  }
  if ((ds_2 = mq_open(QUEUE_NAME_2,
                      O_CREAT | O_RDONLY, 0600,
                      &queue_attr_2)) == (mqd_t)-1)
  {
    perror("Creating queue error");
    return -1;
  }

  char buffer[SIZE + 1];
  char new_text[SIZE + 1];
  int flag = 1;
  unsigned int prio;

  while (flag)
  {
    get_msg(buffer);
    if (strcmp(buffer, "exit") == 0)
    {
      flag = 0;
      break;
    }
    if (mq_send(ds_1, buffer, strlen(buffer), PRIORITY) == -1)
    {
      perror("Sending message error");
      return -1;
    }
    if (mq_receive(ds_2, new_text, SIZE, &prio) == -1)
    {
      perror("cannot receive");
      return -1;
    }
    printf("Message: %s\n", new_text);
  }

  if (mq_close(ds_1) == -1)
    perror("Closing queue error");
  if (mq_close(ds_2) == -1)
    perror("Closing queue error");
  mq_unlink(QUEUE_NAME_1);
  mq_unlink(QUEUE_NAME_2);
  return 0;
}

int pong()
{
  signal(SIGINT, sigint_call);
  mqd_t ds_1, ds_2;
  struct mq_attr queue_attr_1, queue_attr_2;

  init_queue_attr(&queue_attr_1);
  init_queue_attr(&queue_attr_2);
  if ((ds_1 = mq_open(QUEUE_NAME_1,
                      O_CREAT | O_RDONLY, 0600,
                      &queue_attr_1)) == (mqd_t)-1)
  {
    perror("Creating queue error");
    return -1;
  }
  if ((ds_2 = mq_open(QUEUE_NAME_2,
                      O_CREAT | O_WRONLY, 0600,
                      &queue_attr_2)) == (mqd_t)-1)
  {
    perror("Creating queue error");
    return -1;
  }

  char buffer[SIZE + 1];
  char new_text[SIZE + 1];
  int flag = 1;
  unsigned int prio;

  while (flag)
  {
    if (mq_receive(ds_1, new_text, SIZE, &prio) == -1)
    {
      perror("cannot receive");
      return -1;
    }
    printf("Message: %s\n", new_text);
    get_msg(buffer);
    if (strcmp(buffer, "exit") == 0)
    {
      flag = 0;
    }
    if (mq_send(ds_2, buffer, strlen(buffer), PRIORITY) == -1)
    {
      perror("Sending message error");
      return -1;
    }
  }

  if (mq_close(ds_1) == -1)
    perror("Closing queue error");
  if (mq_close(ds_2) == -1)
    perror("Closing queue error");
  mq_unlink(QUEUE_NAME_1);
  mq_unlink(QUEUE_NAME_2);
  return 0;
}
