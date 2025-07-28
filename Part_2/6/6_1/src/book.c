#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "book.h"

void initialize_book(book *book)
{
  book->current_id = 0;
  book->head = NULL;
}

node *create_person(char *first_name, char *last_name, int id)
{
  node *new_node = (node *)malloc(sizeof(node));
  new_node->id = id;
  strcpy(new_node->full_name.first_name, first_name);
  strcpy(new_node->full_name.last_name, last_name);
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

// void put_after(book *book, node *original, node *new)
void put_after(node *original, node *new)
{
  new->prev = original;
  new->next = original->next;

  original->next = new;

  if (new->next != NULL)
  {
    new->next->prev = new;
  }
}

void put_before(book *book, node *original, node *new)
{
  new->next = original;
  new->prev = original->prev;

  original->prev = new;

  if (new->prev == NULL)
    book->head = new;
  else
    new->prev->next = new;
}

int is_bigger(node *n_1, node *n_2)
{
  int first_name_bigger = strcmp(n_1->full_name.first_name, n_2->full_name.first_name);
  if (first_name_bigger > 0)
    return 1;
  if (first_name_bigger < 0)
    return 0;
  int last_name_bigger = strcmp(n_1->full_name.last_name, n_2->full_name.last_name);
  if (last_name_bigger > 0)
    return 1;
  return 0;
}

node *put_person(book *book, node *person)
{
  node *cur_node = book->head;
  if (cur_node == NULL)
  {
    book->head = person;
    return person;
  }

  node *last = NULL;
  while (cur_node != NULL)
  {
    if (is_bigger(cur_node, person))
    {
      put_before(book, cur_node, person);
      return person;
    }
    last = cur_node;
    cur_node = cur_node->next;
  }
  put_after(last, person);

  return person;
}

node *pull_person(book *book, node *person)
{
  node *prev = person->prev;
  node *next = person->next;
  if (prev != NULL)
  {
    prev->next = next;
  }
  else
  {
    book->head = next;
  }
  if (next != NULL)
  {
    next->prev = prev;
  }
  return person;
}

node *add_person(book *book, char *first_name, char *last_name)
{
  node *new_node = create_person(first_name, last_name, book->current_id);
  book->current_id++;

  return put_person(book, new_node);
}

node *find_person_by_id(book book, int id)
{
  node *cur_node = book.head;
  while (cur_node != NULL)
  {
    if (cur_node->id == id)
      return cur_node;
    cur_node = cur_node->next;
  }

  return NULL; // Not found
}

node *find_person_by_name(book book, char *name, info_type name_type)
{

  node *cur_node = book.head;
  switch (name_type)
  {
  case FIRST_NAME:
    while (cur_node != NULL)
    {
      if (strcmp(cur_node->full_name.first_name, name) == 0)
        return cur_node;
      cur_node = cur_node->next;
    }
    break;

  case LAST_NAME:
    while (cur_node != NULL)
    {
      if (strcmp(cur_node->full_name.last_name, name) == 0)
        return cur_node;
      cur_node = cur_node->next;
    }
    break;

  default:
    return NULL;
  }

  return NULL; // Not found
}

int edit_person(book *book, node *person, info_type type, char *new_string)
{
  if (person == NULL)
    return 1;
  pull_person(book, person);
  switch (type)
  {
  case FIRST_NAME:
    strcpy(person->full_name.first_name, new_string);
    break;

  case LAST_NAME:
    strcpy(person->full_name.last_name, new_string);
    break;

  case PHONE:
    strcpy(person->misc_info.phone_number, new_string);
    break;

  case EMAIL:
    strcpy(person->misc_info.email, new_string);
    break;
  default:
    break;
  }
  put_person(book, person);
  return 0;
}

int delete_person(book *book, node *person)
{
  if (person == NULL)
    return 1; // ERROR
  pull_person(book, person);
  free(person);
  return 0;
}

void delete_all(book *book)
{
  node *cur_node = book->head;
  node *next = NULL;

  while (cur_node != NULL)
  {
    next = cur_node->next;
    free(cur_node);
    cur_node = next;
  }

  book->head = NULL;
}
