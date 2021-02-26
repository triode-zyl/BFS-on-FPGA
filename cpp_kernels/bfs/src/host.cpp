#include "xcl2.hpp"
#include <vector>
#include "bfs.h"
#include <fstream>
#include <chrono>
#include <queue>

#define ROOT 0

void bfs(std::vector<int> list[NODE_NUM], int node, int &level)
{
  std::queue<int> Q;
  int flag[NODE_NUM];
  memset(flag,0,sizeof(int)*NODE_NUM);
  Q.push(ROOT);
  level=0;
  while(!Q.empty())
  {
    int head=Q.front();
    Q.pop();
    for(std::vector<int>::iterator i=list[head].begin();i!=list[head].end();i++)
    {
      //cout<<*i<<endl;
      /*if(*i==node)
        break;*/
      if(flag[*i]==0)
      {
        flag[*i]=1;
        Q.push(*i);
      }
    }
    level++;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
    return EXIT_FAILURE;
  }

  auto binaryFile = argv[1];
/****************************/
/*********DEVICE PART********/
/****************************/
  // Allocate Memory in Host Memory
  cl_int err;
  cl::Context context;
  cl::CommandQueue q;
  cl::Kernel krnl_bfs;

  std::vector<rpa_t, aligned_allocator<rpa_t>> RPA(NODE_NUM);
  std::vector<batch_t, aligned_allocator<batch_t>> CIA(CIA_SIZE/BATCH);
  //std::vector<int, aligned_allocator<int>> source_sw_results(DATA_SIZE);

  std::ifstream fin;
  fin.open("../../../RPA.txt");
  //if(!fin)
    //std::cout<<"open error"<<std::endl;
  for(int i=0;i<NODE_NUM;i++)
  {
    int start,num;
    fin>>start>>num;
    RPA[i].range(31,0)=start;
    RPA[i].range(63,32)=num;
    //fin>>RPA[i][0]>>RPA[i+1][1];
  }
  fin.close();
  fin.open("../../../CIA.txt");
  for(int i=0;i<CIA_SIZE/BATCH;i++)
  {
    short cia[BATCH];
    for(int j=0;j<BATCH;j++)
    {
      fin>>cia[j];
      CIA[i].range((j+1)*16-1,j*16)=cia[j];
    }
  }
  fin.close();
  // OPENCL HOST CODE AREA START
  auto devices = xcl::get_xil_devices();

  // Create Program and Kernel
  auto fileBuf = xcl::read_binary_file(binaryFile);
  cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
  bool valid_device = false;
  for (unsigned int i = 0; i < devices.size(); i++) {
    auto device = devices[i];
    // Creating Context and Command Queue for selected Device
    OCL_CHECK(err, context = cl::Context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                        CL_QUEUE_PROFILING_ENABLE, &err));

    std::cout << "Trying to program device[" << i
              << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    cl::Program program(context, {device}, bins, NULL, &err);
    if (err != CL_SUCCESS) {
      std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
    } else {
      std::cout << "Device[" << i << "]: program successful!\n";
      OCL_CHECK(err, krnl_bfs = cl::Kernel(program, "bfs", &err));
      valid_device = true;
      break; // we break because we found a valid device
    }
  }
  if (!valid_device) {
    std::cout << "Failed to program any device found, exit!\n";
    exit(EXIT_FAILURE);
  }

  // Allocate Buffer in Global Memory
  OCL_CHECK(err, cl::Buffer buffer_input1(
                     context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                     NODE_NUM*sizeof(rpa_t), RPA.data(), &err));
  OCL_CHECK(err, cl::Buffer buffer_input2(
                     context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                     CIA_SIZE/BATCH*sizeof(batch_t), CIA.data(), &err));


  // Set the Kernel Arguments
  int narg = 0;
  OCL_CHECK(err, err = krnl_bfs.setArg(narg++, buffer_input1));
  OCL_CHECK(err, err = krnl_bfs.setArg(narg++, buffer_input2));
  OCL_CHECK(err, err = krnl_bfs.setArg(narg++, ROOT));
  //OCL_CHECK(err, err = krnl_bfs.setArg(narg++, size));

  // Copy input data to device global memory
  OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input1,buffer_input2},
                                                  0 /* 0 means from host*/));
	auto begin = std::chrono::high_resolution_clock::now();
  // Launch the Kernel
  //for(int i=0;i<1000;i++)
  //{
  OCL_CHECK(err, err = q.enqueueTask(krnl_bfs));
/*
  // Copy Result from Device Global Memory to Host Local Memory
  OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output},
                                                  CL_MIGRATE_MEM_OBJECT_HOST));*/
  q.finish();
  //}
	auto end = std::chrono::high_resolution_clock::now();
  double device_elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  std::cout<<"Time on device: "<<device_elapsedTime<<" ms"<<std::endl;
  // OPENCL HOST CODE AREA END

/****************************/
/*********HOST PART**********/
/****************************/

  fin.open("../../data_example.txt");
  //if(!fin)
  //  std::cout<<"open error"<<std::endl;
  std::vector<int> list[NODE_NUM];
  int a,b;
  while(fin>>a>>b)
    list[a].push_back(b);
    int node;
    //cin>>node;
    int level;

  begin = std::chrono::high_resolution_clock::now();
  // Launch the Kernel
  //for(int i=0;i<1000;i++)
    bfs(list,node,level);
	end = std::chrono::high_resolution_clock::now();
  double host_elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  std::cout<<"Time on host: "<<host_elapsedTime<<" ms"<<std::endl;
  std::cout<<"Speedup ratio: "<<host_elapsedTime/device_elapsedTime<<std::endl;
  return 0;
}
