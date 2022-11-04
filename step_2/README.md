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

Afterward, I had trouble with getting CUDA to play nice with DPC++. So I went with straight cuda to make sure that works.

I am currently making a cuda.cpp that will compile with a cuda version of a sha256 hasher.

It seems I could have install Intel One instead of the intel llvm stuff. Once done, my SYCL hello program started working. Now to get back to non-CUDA sycl stuff (and get the code to run on the nVidia GPU instead of Intel CPU).

But the stuff installed by the package manager is too old. But I did make progress. The hello program can now run on with my GPU.

Now to try SHA256
