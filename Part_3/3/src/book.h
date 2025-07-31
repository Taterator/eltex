#ifndef BOOK_H_
#define BOOK_H_

#define NAME_CHARS 20
#define MAX_NODES 100

typedef enum
{
  FIRST_NAME,
  LAST_NAME,
  PHONE,
  EMAIL
} info_type;

typedef struct
{
  char first_name[NAME_CHARS];
  char last_name[NAME_CHARS];
} full_name;

typedef struct
{
  char phone_number[NAME_CHARS];
  char email[NAME_CHARS];
} misc_info;

typedef struct
{
  int id;
  full_name full_name;
  misc_info misc_info;
} node;

typedef struct
{
  int count;
  node nodes[MAX_NODES];
  int current_id;
} book;

void initialize_book(book *book);
int add_person(book *book, char *first_name, char *last_name);
int find_person_by_id(book book, int id);
int find_person_by_name(book book, char *name, info_type name_type);
int delete_person(book *book, int person_id);
int edit_person(book *book, int index, info_type type, char *new_string);

#endif