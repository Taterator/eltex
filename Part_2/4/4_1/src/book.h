#ifndef BOOK_H_
#define BOOK_H_

#define NAME_CHARS 20

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

typedef struct node_
{
  int id;
  full_name full_name;
  misc_info misc_info;
  struct node_ *next;
  struct node_ *prev;
} node;

typedef struct
{
  node *head;
  int current_id;
} book;

void initialize_book(book *book);
node *add_person(book *book, char *first_name, char *last_name);
node *find_person_by_id(book book, int id);
node *find_person_by_name(book book, char *name, info_type name_type);
int delete_person(book *book, node *person);
int edit_person(book *book, node *person, info_type type, char *new_string);
void delete_all(book *book);

#endif