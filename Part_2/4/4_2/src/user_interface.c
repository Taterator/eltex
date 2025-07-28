#include <stdio.h>
#include "queue.h"

void print_node(node_t node)
{
  printf("-> %d | %d | %s\n", node.id,
         node.priority, node.message);
}

void print_node_head(node_t node)
{
  printf("-> id | priority | message\n");
  print_node(node);
}

void print_queue(queue queue)
{
  if (queue.head == NULL)
  {
    printf("| NONE\n");
    return;
  }
  node_t *cur_node = queue.head;
  printf("-> id | priority | message\n");
  while (cur_node != NULL)
  {
    print_node(*cur_node);
    cur_node = cur_node->next;
  }
}

int get_command_interface()
{
  printf("| Enter command:\n| 1 - add message, 2 - pop message\n| 3 - find message, 4 - print messages, quit - to exit\n");

  int num = -1;
  printf("> ");
  scanf("%d", &num);
  return num;
}

void add_message_interface(queue *queue)
{
  char message_str[MESSAGE_CHARS];
  int priority = -1;
  printf("| Enter message:\n");
  printf("> ");
  scanf("%s", message_str);
  printf("| Enter priority:\n");
  printf("> ");
  scanf("%d", &priority);
  node_t *message = add_node(queue, priority, message_str);
  if (message == NULL)
  {
    printf("| An error occured while adding\n");
    return;
  }
}

void delete_message_interface(queue *queue)
{
  printf("| Popped message:\n");
  node_t message;
  int res = pop(queue, &message);
  if (res == 1)
  {
    printf("| Error: Head is empty\n");
    return;
  }
  print_node(message);
}

void find_message_interface(queue *queue)
{
  node_t message;
  int res = -1;
  int priority = -1;
  int mode = -1;

  printf("Enter priority:\n");
  printf("> ");
  scanf("%d", &priority);
  printf("Enter mode: 1 - >= (numerically), 2 - ==\n");
  printf("> ");
  scanf("%d", &mode);

  res = pop_message_by_priority(queue, priority, mode, &message);
  if (res == 1)
  {
    printf("| Message not found\n");
    return;
  }
  else if (res == 2)
  {
    printf("| Unknown mode\n");
    return;
  }
  printf("| Message found:\n");
  print_node_head(message);
}

void print_queue_interface(queue queue)
{
  print_queue(queue);
}

int main()
{
  queue queue;
  initialize_queue(&queue);
  int command = 1;
  while (command > 0)
  {
    command = get_command_interface();
    switch (command)
    {
    case 1:
      add_message_interface(&queue);
      break;

    case 2:
      delete_message_interface(&queue);
      break;

    case 3:
      find_message_interface(&queue);
      break;

    case 4:
      print_queue_interface(queue);
      break;
    default:
      break;
    }
  }
  delete_all(&queue);
  return 0;
}