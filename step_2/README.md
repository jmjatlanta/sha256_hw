In step 2, I want to compile the code with DPC++.

To do this, I must install the NVIDA CUDA toolkit and then get the llvm stack from Intel.

`git clone https://github.com/intel/llvm -b sycl sycl` will put it in the `sycl` directory (llvm is too generic for me). After cloning, I compiled using their python buildbot scripts.

```
python buildbot/configure.py --werror --cuda
python buildbot/compile.py -j14
```

That seems to have put the binaries in sysctl/build/bin

With the compilers now set, I modified the Makefile. After recompiling, it seems there is no change in the results or the performance times.

But than I realized I had not installed the CUDA prerequisites. So the above `configure.py` may not have been correct. So I wiped out my sycl directory, installed the prerequisites, and cloned/compiled.




