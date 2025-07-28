#include <string.h>
#include <stdio.h>

#include "book.h"

void initialize_book(book *book)
{
  book->count = 0;
  book->current_id = 0;
}

int add_person(book *book, char *first_name, char *last_name)
{
  if (book->count == MAX_NODES)
    return 1;
  book->nodes[book->count].id = book->current_id;
  strcpy(book->nodes[book->count].full_name.first_name, first_name);
  strcpy(book->nodes[book->count].full_name.last_name, last_name);

  book->count++;
  book->current_id++;
  return 0;
}

int find_person_by_id(book book, int id)
{
  for (int index = 0; index < book.count; index++)
  {
    if (book.nodes[index].id == id)
    {
      return index;
    }
  }

  return -1; // Not found
}

int find_person_by_name(book book, char *name, info_type name_type)
{
  switch (name_type)
  {
  case FIRST_NAME:
    for (int index = 0; index < book.count; index++)
    {
      if (strcmp(book.nodes[index].full_name.first_name, name) == 0)
        return index;
    }
    break;

  case LAST_NAME:
    for (int index = 0; index < book.count; index++)
    {
      if (strcmp(book.nodes[index].full_name.last_name, name) == 0)
        return index;
    }
    break;

  default:
    return -1;
  }

  return -1; // Not found
}

int edit_person(book *book, int index, info_type type, char *new_string)
{
  switch (type)
  {
  case FIRST_NAME:
    strcpy(book->nodes[index].full_name.first_name, new_string);
    break;

  case LAST_NAME:
    strcpy(book->nodes[index].full_name.last_name, new_string);
    break;

  case PHONE:
    strcpy(book->nodes[index].misc_info.phone_number, new_string);
    break;

  case EMAIL:
    strcpy(book->nodes[index].misc_info.email, new_string);
    break;
  default:
    break;
  }
  return 0;
}

int delete_person(book *book, int person_id)
{
  int person_index = find_person_by_id(*book, person_id);
  if (person_index < 0)
    return 1; // ERROR
  for (int i = person_index; i < book->count && i < MAX_NODES - 1; i++)
  {
    book->nodes[i] = book->nodes[i + 1];
  }
  book->count--;
  return 0;
}