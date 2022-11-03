In step 1, the bitcoin implementation of SHA256 is placed in a test harness. This test harness simulates the working
environment that is the eventual goal of the project. It is compiled for the purpose of providing a baseline and
profiling results.

The bitcoin implementation comes with several versions for different CPUs. 

The initial results are promising. With the compiler doing no optimizations, generating 12,800 hashes took more than 56%
of the time. The majority in CSHA256::Finalize and also CSHA256::Write.

The output shows 14211937ns total, averaging 1110ns per loop iteration (without valgrind running).

Using -O2 shows   3295069ns total, averaging  257ns per loop.
