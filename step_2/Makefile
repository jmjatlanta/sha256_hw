CXX=../../sycl/build/bin/clang++
SYCL_DIR=../../sycl/build
FSYCL_PARAMS=-fsycl -fsycl-targets=nvptx64-nvidia-cuda #-fno-sycl-libspirv
#CXX=dpcpp
#SYCL_DIR=/opt/intel/oneapi/compiler/2022.2.0/linux
CXXFLAGS=-O2 -I. -I$(SYCL_DIR)/include/sycl -I$(SYCL_DIR) $(FSYCL_PARAMS)
LFLAGS=$(FSYCL_PARAMS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

%.o: %.cu
	nvcc -c $(CXXFLAGS) $^ -o $@

step2: main.o crypto/sha256.o uint256.o util/strencodings.o
	$(CXX) $(LFLAGS) -o $@ $^

# the "Hello, World! of SYCL/DPC++
hello: hello.o
	$(CXX) $(LFLAGS) -o $@ -lsycl $^

# the "hello world" of cuda
vector_add: vector_add.cu
	nvcc $^ -o $@

# a sha256 implementation in cuda
cuda: cuda.cu
	nvcc $^ -o $@

clean:
	$(RM) *.o
	$(RM) crypto/*.o
	$(RM) util/*.o
	$(RM) step2
	$(RM) hello
	$(RM) cuda
	$(RM) vector_add
