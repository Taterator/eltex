#ifndef QUEUE_H_
#define QUEUE_H_

#define MESSAGE_CHARS 100

typedef struct node_
{
  int id;
  int priority;
  char message[MESSAGE_CHARS];
  struct node_ *next;
  struct node_ *prev;
} node_t;

typedef struct
{
  node_t *head;
  node_t *tail;
  int current_id;
} queue;

void initialize_queue(queue *queue);
node_t *add_node(queue *queue, int priority, char *message);
int pop(queue *queue, node_t *result);
int pop_message_by_priority(queue *queue, int priority, int mode, node_t *result);
int delete_node(queue *queue, node_t *message);
void delete_all(queue *queue);

#endif