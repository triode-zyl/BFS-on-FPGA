
// Includes
#include <ap_int.h>
#include <hls_stream.h>
#include "bfs.h"
#include <string.h>

#define BANK_BW 4
// TRIPCOUNT identifier
const int c_size = NODE_NUM;
const int cia_depth = CIA_SIZE/BATCH;
const int next_frontier_depth = NODE_NUM/BATCH;

char bitmap0[NODE_NUM/BATCH];
char bitmap1[NODE_NUM/BATCH];
char bitmap2[NODE_NUM/BATCH];
char bitmap3[NODE_NUM/BATCH];
char bitmap4[NODE_NUM/BATCH];
char bitmap5[NODE_NUM/BATCH];
char bitmap6[NODE_NUM/BATCH];
char bitmap7[NODE_NUM/BATCH];
char bitmap8[NODE_NUM/BATCH];
char bitmap9[NODE_NUM/BATCH];
char bitmap10[NODE_NUM/BATCH];
char bitmap11[NODE_NUM/BATCH];
char bitmap12[NODE_NUM/BATCH];
char bitmap13[NODE_NUM/BATCH];
char bitmap14[NODE_NUM/BATCH];
char bitmap15[NODE_NUM/BATCH];


static void read_frontier(short *frontier, hls::stream<short> &frontier_stream, short size)
{
read1:
  for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = 1 max = c_size
    // Blocking write command to inStream
    frontier_stream << frontier[i];
  }
}

static void read_rpa(rpa_t *RPA, hls::stream<short> &frontier_stream, hls::stream<rpa_t> &rpa_stream, short size)
{
read2:
  for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = 1 max = c_size
    int vertexIdx = frontier_stream.read();
		rpa_t data = RPA[vertexIdx];
		rpa_stream << data;
  }  
}

static void read_cia(batch_t *CIA, hls::stream<rpa_t> &rpa_stream, hls::stream<batch_t> &cia_stream, short size)
{
read3:
	for(int i = 0; i < size; i++){
#pragma HLS LOOP_TRIPCOUNT min = 1 max = cia_depth
		rpa_t word  = rpa_stream.read();
		int start = word.range(31,0) >> BANK_BW;
    int num = word.range(63,32) >> BANK_BW;
		for(int j = 0; j < num; j++){
			cia_stream << CIA[start + j];
		}
	}
}

static void traverse_cia(hls::stream<short> next_frontier_stream[], hls::stream<batch_t> &cia_stream, short *next_frontier_size)
{
  while(!cia_stream.empty())
  {
    batch_t word = cia_stream.read();
  #pragma HLS PIPELINE II=1
  word0:
    if(word[0]!=-1)
    {
      short tmp=word.range(15,0) >> BANK_BW;
      if(bitmap0[tmp]==0)
      {
        bitmap0[tmp]=1;
        next_frontier_stream[0] << word.range(15,0);
        next_frontier_size[0]++;
      }
    }
  word1:
    if(word.range(31,16)!=-1)
    {
      short tmp=word.range(31,16) >> BANK_BW;
      if(bitmap1[tmp]==0)
      {
        bitmap1[tmp]=1;
        next_frontier_stream[1] << word.range(31,16);
        next_frontier_size[1]++;
      }
    }
  word2:
    if(word.range(47,32)!=-1)
    {
      short tmp=word.range(47,32) >> BANK_BW;
      if(bitmap2[tmp]==0)
      {
        bitmap2[tmp]=1;
        next_frontier_stream[2] << word.range(47,32);
        next_frontier_size[2]++;
      }
    }
  word3:
    if(word.range(63,48)!=-1)
    {
      short tmp=word.range(63,48) >> BANK_BW;
      if(bitmap3[tmp]==0)
      {
        bitmap3[tmp]=1;
        next_frontier_stream[3] << word.range(63,48);
        next_frontier_size[3]++;
      }
    }
  word4:
    if(word.range(79,64)!=-1)
    {
      short tmp=word.range(79,64) >> BANK_BW;
      if(bitmap4[tmp]==0)
      {
        bitmap4[tmp]=1;
        next_frontier_stream[4] << word.range(79,64);
        next_frontier_size[4]++;
      }
    }
  word5:
    if(word.range(95,80)!=-1)
    {
      short tmp=word.range(95,80) >> BANK_BW;
      if(bitmap5[tmp]==0)
      {
        bitmap5[tmp]=1;
        next_frontier_stream[5] << word.range(95,80);
        next_frontier_size[5]++;
      }
    }
  word6:
    if(word.range(111,96)!=-1)
    {
      short tmp=word.range(111,96) >> BANK_BW;
      if(bitmap6[tmp]==0)
      {
        bitmap6[tmp]=1;
        next_frontier_stream[6] << word.range(111,96);
        next_frontier_size[6]++;
      }
    }
  word7:
    if(word.range(127,112)!=-1)
    {
      short tmp=word.range(127,112) >> BANK_BW;
      if(bitmap7[tmp]==0)
      {
        bitmap7[tmp]=1;
        next_frontier_stream[7] << word.range(127,112);
        next_frontier_size[7]++;
      }
    }
  word8:
    if(word.range(143,128)!=-1)
    {
      short tmp=word.range(143,128) >> BANK_BW;
      if(bitmap8[tmp]==0)
      {
        bitmap8[tmp]=1;
        next_frontier_stream[8] << word.range(143,128);
        next_frontier_size[8]++;
      }
    }
  word9:
    if(word.range(159,144)!=-1)
    {
      short tmp=word.range(159,144) >> BANK_BW;
      if(bitmap9[tmp]==0)
      {
        bitmap9[tmp]=1;
        next_frontier_stream[9] << word.range(159,144);
        next_frontier_size[9]++;
      }
    }
  word10:
    if(word.range(175,160)!=-1)
    {
      short tmp=word.range(175,160) >> BANK_BW;
      if(bitmap10[tmp]==0)
      {
        bitmap10[tmp]=1;
        next_frontier_stream[10] << word.range(175,160);
        next_frontier_size[10]++;
      }
    }
  word11:
    if(word.range(191,176)!=-1)
    {
      short tmp=word.range(191,176) >> BANK_BW;
      if(bitmap11[tmp]==0)
      {
        bitmap11[tmp]=1;
        next_frontier_stream[11] << word.range(191,176);
        next_frontier_size[11]++;
      }
    }
  word12:
    if(word.range(207,192)!=-1)
    {
      short tmp=word.range(207,192) >> BANK_BW;
      if(bitmap12[tmp]==0)
      {
        bitmap12[tmp]=1;
        next_frontier_stream[12] << word.range(207,192);
        next_frontier_size[12]++;
      }
    }
  word13:
    if(word.range(223,208)!=-1)
    {
      short tmp=word.range(223,208) >> BANK_BW;
      if(bitmap13[tmp]==0)
      {
        bitmap13[tmp]=1;
        next_frontier_stream[13] << word.range(223,208);
        next_frontier_size[13]++;
      }
    }
  word14:
    if(word.range(239,224)!=-1)
    {
      short tmp=word.range(239,224) >> BANK_BW;
      if(bitmap14[tmp]==0)
      {
        bitmap14[tmp]=1;
        next_frontier_stream[14] << word.range(239,224);
        next_frontier_size[14]++;
      }
    }
  word15:
    if(word.range(255,240)!=-1)
    {
      short tmp=word.range(255,240) >> BANK_BW;
      if(bitmap15[tmp]==0)
      {
        bitmap15[tmp]=1;
        next_frontier_stream[15] << word.range(255,240);
        next_frontier_size[15]++;
      }
    }
  }
}


extern "C" {

void bfs(rpa_t *RPA, batch_t *CIA, int ROOT) {

  static hls::stream<rpa_t> RPA_stream("input_stream1");
  static hls::stream<batch_t> CIA_stream("input_stream2");
#pragma HLS STREAM variable = RPA_stream depth = c_size
#pragma HLS STREAM variable = CIA_stream depth = cia_depth
  static hls::stream<short> frontier_stream("frontier_stream");
  static hls::stream<short> next_frontier_stream[BATCH];
#pragma HLS STREAM variable = frontier_stream depth = c_size
#pragma HLS STREAM variable = next_frontier_stream depth = next_frontier_depth

  short frontier[NODE_NUM];
  short next_frontier_size[BATCH];
  short frontier_size;
  
  memset(bitmap0,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap1,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap2,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap3,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap4,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap5,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap6,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap7,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap8,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap9,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap10,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap11,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap12,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap13,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap14,0,sizeof(char)*NODE_NUM/BATCH);
  memset(bitmap15,0,sizeof(char)*NODE_NUM/BATCH);

  memset(next_frontier_size,0,sizeof(int)*BATCH);
  frontier[0]=0;
  frontier_size=1;
  
  while(frontier_size>0)
  {
  #pragma HLS dataflow
    read_frontier(frontier, frontier_stream, frontier_size);
    read_rpa(RPA, frontier_stream, RPA_stream, frontier_size);
    read_cia(CIA, RPA_stream, CIA_stream, frontier_size);
    traverse_cia(next_frontier_stream, CIA_stream, next_frontier_size);
    frontier_size=0;
    int baseVec[BATCH];
    baseVec[0]=0;
		for(int i = 1; i < BATCH; i++){
			baseVec[i] = baseVec[i - 1] + next_frontier_size[i - 1];
		}
    frontier_size=baseVec[BATCH-1]+next_frontier_size[BATCH-1];
    for(int i=0;i<BATCH;i++)
    {
    #pragma HLS UNROLL
      for(int j=0;j<next_frontier_size[i];j++)
      {
        frontier[baseVec[i]+j]=next_frontier_stream[i].read();
      }
    }
  }
}
}
