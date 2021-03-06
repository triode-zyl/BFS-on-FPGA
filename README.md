# BFS on FPGA

## Introduction

In our final project, there is a part of the algorithm whose principle is similar to BFS, so I plan to accelerate a simple BFS algorithm with FPGA. If there is still room, use the accelerated BFS algorithm as a template to make modifications to the corresponding part of our project for the purpose of accelerating the project with FPGA.

## Environment

g++ (GCC) 9.3.0

Ubuntu 18.04.2

Test FPGA boards: Xilinx Alveo U280

## Theory

Refer to [BFS-on-FPGA.pptx](BFS-on-FPGA.pptx)

## Compile

```shell
cd cpp_kernels/bfs
make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>
make run TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> HOST_ARCH=<aarch32/aarch64/x86> EDGE_COMMON_SW=<rootfs and kernel image path>
```

## Result

I test the program by using the software emulation, here are the compiler instructions:

```shell
cd cpp_kernels/bfs
make all TARGET=sw_emu DEVICE=xilinx_u280_xdma_201920_3
make run TARGET=sw_emu DEVICE=xilinx_u280_xdma_201920_3
```

Here's the emulation result:

```
cp -rf ./_x.sw_emu.xilinx_u280_xdma_201920_3/emconfig.json .
XCL_EMULATION_MODE=sw_emu ./dataflow_stream ./build_dir.sw_emu.xilinx_u280_xdma_201920_3/bfs.xclbin
Found Platform
Platform Name: Xilinx
INFO: Reading ./build_dir.sw_emu.xilinx_u280_xdma_201920_3/bfs.xclbin
Loading: './build_dir.sw_emu.xilinx_u280_xdma_201920_3/bfs.xclbin'
Trying to program device[0]: xilinx_u280_xdma_201920_3
Device[0]: program successful!
Time on device: 19 ms
Time on host: 12 ms
Speedup ratio: 0.571429
```

Though I didn't test the program on FPGA boards, I believe there still are improving spaces. I will further optimize my codes in the future.

Result on FPGA:

```
./dataflow_stream ./build_dir.hw.xilinx_u280_xdma_201920_3/bfs.xclbin
Found Platform
Platform Name: Xilinx
INFO: Reading ./build_dir.hw.xilinx_u280_xdma_201920_3/bfs.xclbin
Loading: './build_dir.hw.xilinx_u280_xdma_201920_3/bfs.xclbin'
Trying to program device[0]: xilinx_u280_xdma_201920_3
Device[0]: program successful!
Time on device: 10 ms
Time on host: 12 ms
Speedup ratio: 1.2
```

## References

https://github.com/Xilinx/Vitis_Accel_Examples

https://github.com/Liu-Cheng/bfs_with_Intel_OpenCL

