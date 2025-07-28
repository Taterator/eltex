#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void initialize_queue(queue *queue)
{
  queue->current_id = 0;
  queue->head = NULL;
  queue->tail = NULL;
}

node_t *create_node(char *message, int priority, int id)
{
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->id = id;
  new_node->priority = priority;
  strcpy(new_node->message, message);
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

void put_after(queue *queue, node_t *original, node_t *new)
{
  new->prev = original;
  new->next = original->next;

  original->next = new;

  if (new->next == NULL)
    queue->tail = new;
  else
    new->next->prev = new;
}

void put_before(queue *queue, node_t *original, node_t *new)
{
  new->next = original;
  new->prev = original->prev;

  original->prev = new;

  if (new->prev == NULL)
    queue->head = new;
  else
    new->prev->next = new;
}

int is_bigger(node_t *n_1, node_t *n_2)
{
  return n_1->priority > n_2->priority;
}

node_t *put_node(queue *queue, node_t *node)
{
  node_t *cur_node = queue->head;
  if (cur_node == NULL)
  {
    queue->head = node;
    queue->tail = node;
    return node;
  }

  node_t *last = NULL;
  while (cur_node != NULL)
  {
    if (is_bigger(cur_node, node))
    {
      put_before(queue, cur_node, node);
      return node;
    }
    last = cur_node;
    cur_node = cur_node->next;
  }
  put_after(queue, last, node);

  return node;
}

node_t *pull_node(queue *queue, node_t *node)
{
  if (node == NULL)
    return NULL;
  node_t *prev = node->prev;
  node_t *next = node->next;
  if (prev != NULL)
  {
    prev->next = next;
  }
  else
  {
    queue->head = next;
  }
  if (next != NULL)
  {
    next->prev = prev;
  }
  else
  {
    queue->tail = next;
  }
  return node;
}

node_t *add_node(queue *queue, int priority, char *message)
{
  if (priority > 255 || priority < 0)
    return NULL;
  node_t *new_node = create_node(message, priority, queue->current_id);
  queue->current_id++;

  return put_node(queue, new_node);
}

int pop_message_by_priority(queue *queue, int priority, int mode, node_t *result)
{
  node_t *cur_node = queue->head;
  switch (mode)
  {
  case 1:
    while (cur_node != NULL)
    {
      if (cur_node->priority >= priority)
      {
        *result = *cur_node;
        free(pull_node(queue, cur_node));
        return 0;
      }
      cur_node = cur_node->next;
    }
    return 1;
    break;

  case 2:
    while (cur_node != NULL)
    {
      if (cur_node->priority == priority)
      {
        *result = *cur_node;
        free(pull_node(queue, cur_node));
        return 0;
      }
      cur_node = cur_node->next;
    }
    return 1;
    break;

  default:
    return 2;
    break;
  }
}

int pop(queue *queue, node_t *result)
{
  node_t *node_p = pull_node(queue, queue->head);
  if (node_p == NULL)
    return 1;
  *result = *node_p;
  free(node_p);
  return 0;
}

int delete_node(queue *queue, node_t *node)
{
  if (node == NULL)
    return 1; // ERROR
  pull_node(queue, node);
  free(node);
  return 0;
}

void delete_all(queue *queue)
{
  node_t *cur_node = queue->head;
  node_t *next = NULL;

  while (cur_node != NULL)
  {
    next = cur_node->next;
    free(cur_node);
    cur_node = next;
  }

  queue->head = NULL;
  queue->tail = NULL;
}
