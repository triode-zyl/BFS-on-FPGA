#ifndef BFS_H_
#define BFS_H_

#include<ap_int.h>

#define BATCH 16
#define BANK_BW 4
#define NODE_NUM 16384
#define CIA_SIZE 1014784

typedef ap_uint<64> rpa_t;
typedef ap_uint<256> batch_t;

//typedef int rpa_t[2];
//typedef short batch_t[BATCH];

#endif