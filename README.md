## Performance of SHA256 with Hardware Accelleration

The standard implementation of SHA256 within bitcoin core code (using 18.0 ish) is fairly well tuned for Intel processors.
I would like to use this as a baseline.

The need is to compute very specific hashes and compare them to a threshold, just as bitcoin does with their difficulty 
settings. At each round, there were about 12,800 runs of a loop (similar to the nonce of bitcoin core). On one thread, 
this ran in about 20 seconds. Using a thread pool and an Intel i7 with 8 CPUs and 16 cores, this was reduced to 6 seconds. 
I would like to get this number down further.

There are a few steps to develop a baseline.

1. Segregate the code to just do the hash and compare process.
1. Run this code through a profiler and look for optimizations. Based on those observations, determine what would be the
benefit of offloading the SHA256 computation from the CPU to a GPU/FPGA/ASIC.

## Other Goals

PCs with Intel and AMD GPUs are common. Moving some repetative part of the hash and compare process to the GPU would 
probably be of benefit. But I would like to be able to write for both Intel and AMD GPUs within the same codebase. 
I would like to take the opportunity in this project to look at Intel's DPC++ and SYCL to attempt to keep a unified 
codebase.
