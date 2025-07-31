#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "book.h"

static char const *file_name = "data_file.txt";

// int get_line_info(book book, int index, char *buffer, int buffer_len)
// {
//   snprintf(buffer, buffer_len, "%s,%s,%s,%s\n", book.nodes[index].full_name.first_name, book.nodes[index].full_name.last_name,
//            book.nodes[index].misc_info.email, book.nodes[index].misc_info.phone_number);
//   return 0;
// }

int save_to_file(book book)
{
  int file = open(file_name, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

  if (file == -1)
  {
    printf("Could not open the file %s", file_name);
    return 0;
  }

  write(file, &book.current_id, sizeof(int));

  for (int i = 0; i < book.count; i++)
  {
    write(file, &(book.nodes[i]), sizeof(node));
  }

  close(file);

  return 0;
}

int read_from_file(book *book)
{
  int file = open(file_name, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);

  if (file == -1)
  {
    printf("Could not open the file %s", file_name);
    return 1;
  }
  int count = 0;
  int cur_id = 0;

  count = read(file, &cur_id, sizeof(int));
  if (count == -1)
  {
    perror("Read error\n");
    close(file);
    return 1;
  }
  book->current_id = cur_id;

  node person;

  while ((count = read(file, &person, sizeof(node))) != 0)
  {
    if (count == -1)
    {
      perror("Read error\n");
      close(file);
      return 1;
    }
    book->nodes[book->count] = person;
    if (count != sizeof(node))
    {
      fprintf(stderr, "Size mismatch\n");
      break;
    }
    book->count++;
  }

  close(file);

  return 0;
}
