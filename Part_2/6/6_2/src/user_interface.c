#include <stdio.h>
#include "book.h"

void print_person(node person)
{
  printf("-> %d | %s | %s | %s | %s\n", person.id,
         person.full_name.first_name,
         person.full_name.last_name,
         person.misc_info.phone_number,
         person.misc_info.email);
}

void print_person_head(node person)
{
  printf("-> id | first name | last name | phone | email\n");
  print_person(person);
}

void print_book(book book)
{
  if (book.head == NULL)
  {
    printf("| NONE\n");
    return;
  }
  node *cur_node = book.head;
  printf("-> id | first name | last name | phone | email\n");
  while (cur_node != NULL)
  {
    print_person(*cur_node);
    cur_node = cur_node->next;
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
  node *person = add_person(book, first_name, last_name);
  if (person == NULL)
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
    if (edit_person(book, person, PHONE, buffer) != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  else
  {
    if (edit_person(book, person, PHONE, "-") != 0)
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
    if (edit_person(book, person, EMAIL, buffer) != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  else
  {
    if (edit_person(book, person, EMAIL, "-") != 0)
    {
      printf("| An error occured while adding\n");
      return;
    }
  }
  // sort(book);
}

void edit_person_interface(book *book)
{
  int id = -1;
  int choice = -1;
  char buffer[NAME_CHARS];
  printf("Enter person id:\n");
  printf("> ");
  scanf("%d", &id);
  node *person = find_person_by_id(*book, id);
  if (person == NULL)
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
    if (edit_person(book, person, FIRST_NAME, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
    // sort(book);
  }
  printf("| Edit last name?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter last name:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, person, LAST_NAME, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
    // sort(book);
  }
  printf("| Edit phone number?\n| 1 - yes, 0 - no\n");
  printf("> ");
  scanf("%d", &choice);
  if (choice == 1)
  {
    printf("| Enter phone number:\n");
    printf("> ");
    scanf("%s", buffer);
    if (edit_person(book, person, PHONE, buffer) != 0)
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
    if (edit_person(book, person, EMAIL, buffer) != 0)
    {
      printf("| An error occured while editing\n");
      return;
    }
  }
}

void delete_person_interface(book *book)
{
  int id = -1;
  printf("Enter person id:\n");
  printf("> ");
  scanf("%d", &id);
  node *person = find_person_by_id(*book, id);
  if (person == NULL)
  {
    printf("| Person not found\n");
    return;
  }
  if (delete_person(book, person) != 0)
  {
    printf("| An error occured while adding\n");
  }
}

void find_person_interface(book *book)
{
  node *person;
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
    person = find_person_by_name(*book, buffer, choice - 1);
    if (person == NULL)
    {
      printf("| Person not found\n");
      return;
    }
    printf("Person found:\n");
    print_person_head(*person);
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
  delete_all(&phone_book);
  return 0;
}