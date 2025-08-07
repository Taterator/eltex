#ifndef COMMON_H_
#define COMMON_H_

#define LINE_COUNT 20

#define FILENAME "text_file"

union semun {
  int val;               /* used for SETVAL only */
  struct semid_ds *buf;  /* for IPC_STAT and IPC_SET */
  unsigned short *array; /* used for GETALL and SETALL */
};

void printer();
void reader();

#endif