#include <stdio.h>
#include "book.h"

void print_person(book book, int index)
{
  if (index < 0)
  {
    printf("| NULL PRINT\n");
    return;
  }
  printf("-> %d | %s | %s | %s | %s\n", book.nodes[index].id,
         book.nodes[index].full_name.first_name,
         book.nodes[index].full_name.last_name,
         book.nodes[index].misc_info.phone_number,
         book.nodes[index].misc_info.email);
}

void print_person_head(book book, int index)
{
  printf("-> id | first name | last name | phone | email\n");
  print_person(book, index);
}

void print_book(book book)
{
  if (book.count == 0)
  {
    printf("NONE");
  }
  printf("-> id | first name | last name | phone | email\n");
  for (int i = 0; i < book.count; i++)
  {
    print_person(book, i);
  }
}

int get_command_interface()
{
  printf("| Enter command:\n| 1 - add user, 2 - edit user, 3 - delete user\n| 4 - find person, 5 - print book, quit - to exit\n");

  int num = -1;
  printf("> ");
  scanf("%d", &num);
  return num;
}

void add_person_interface(book *book)
{
  int choice = -1;
  char buffer[NAME_CHARS];
  char first_name[NAME_CHARS];
  char last_name[NAME_CHARS];
  printf("| Enter first name:\n");
  printf("> ");
  scanf("%s", first_name);
  printf("| Enter last name:\n");
  printf("> ");
  scanf("%s", last_name);
  if (add_person(book, first_name, last_name) != 0)
  {
    printf("| An error occured while adding\n");
    return;
  }
  printf("| Add phone number?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter phone number:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, book->count - 1, PHONE, buffer) != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  else
  {
    if (edit_person(book, book->count - 1, PHONE, "-") != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  printf("| Add email?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter email:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, book->count - 1, EMAIL, buffer) != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  else
  {
    if (edit_person(book, book->count - 1, EMAIL, "-") != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
}

void edit_person_interface(book *book)
{
  int index = -1;
  int id = -1;
  int choice = -1;
  char buffer[NAME_CHARS];
  printf("Enter person id:\n");
  printf("> ");
  scanf("%d", &id);
  index = find_person_by_id(*book, id);
  if (index < 0)
  {
    printf("| Person not found\n");
    return;
  }
  printf("| Edit first name?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter first name:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, index, FIRST_NAME, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
  }
  printf("| Edit last name?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter last name:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, index, LAST_NAME, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
  }
  printf("| Edit phone number?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter phone number:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, index, PHONE, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
  }
  printf("| Edit email?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter email:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, index, EMAIL, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
  }
}

void delete_person_interface(book *book)
{
  int index = -1;
  int id = -1;
  printf("Enter person id:\n");
  printf("> ");
  scanf("%d", &id);
  index = find_person_by_id(*book, id);
  if (index < 0)
  {
    printf("| Person not found\n");
    return;
  }
  if (delete_person(book, index) != 0)
  {
    printf("| An error occured while adding\n");
  }
}

void find_person_interface(book *book)
{
  int index = -1;
  int choice = -1;
  char buffer[NAME_CHARS];
  printf("| Search by first name or last?\n| 1 - first, 2 - last\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1 || choice == 2)
  {
    printf("Enter person name:\n");
    printf("> ");
    scanf("%s", buffer);
    index = find_person_by_name(*book, buffer, choice - 1);
    if (index < 0)
    {
      printf("| Person not found\n");
      return;
    }
    printf("Person found:\n");
    print_person_head(*book, index);
  }
  else
  {
    printf("| Unknown choice\n");
    return;
  }
}

void print_book_interface(book book)
{
  print_book(book);
}

int main()
{
  book phone_book;
  initialize_book(&phone_book);
  int command = 1;
  while (command > 0)
  {
    command = get_command_interface();
    switch (command)
    {
    case 1:
      add_person_interface(&phone_book);
      break;

    case 2:
      edit_person_interface(&phone_book);
      break;

    case 3:
      delete_person_interface(&phone_book);
      break;

    case 4:
      find_person_interface(&phone_book);
      break;

    case 5:
      print_book_interface(phone_book);
      break;
    default:
      break;
    }
  }
  return 0;
}