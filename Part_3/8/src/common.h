#ifndef COMMON_H_
#define COMMON_H_

union semun {
  int val;               /* used for SETVAL only */
  struct semid_ds *buf;  /* for IPC_STAT and IPC_SET */
  unsigned short *array; /* used for GETALL and SETALL */
};

struct sembuf lock_res = {0, -1, 0};
struct sembuf rel_res = {0, 1, 0};
struct sembuf init = {1, 1, 0};
struct sembuf up_prod = {2, 1, 0};
struct sembuf down_prod = {2, -1, 0};
struct sembuf up_fa = {3, 1, 0};
struct sembuf down_fa = {3, -1, 0};

#endif