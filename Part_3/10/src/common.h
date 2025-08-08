#ifndef COMMON_H_
#define COMMON_H_

#define FILENAME "text_file"

union semun {
  int val;               /* used for SETVAL only */
  struct semid_ds *buf;  /* for IPC_STAT and IPC_SET */
  unsigned short *array; /* used for GETALL and SETALL */
};

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

struct shmseg {
  int cnt;
  int complete;
  char buf[BUF_SIZE];
};

void printer();
void reader();

#endif