CXX=../../sycl/build/bin/clang++
CXXFLAGS=-I. -O2 -I../../sycl/build/include/sycl -I../../sycl/build/include -fsycl

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@


step2: main.o crypto/sha256.o uint256.o util/strencodings.o
	$(CXX) $(LFLAGS) -o $@ $^

# the "Hello, World! of SYCL/DPC++
hello: hello.o
	$(CXX) -o $@ $^ $(LFLAGS) -lsycl

# the "hello world" of cuda
vector_add: vector_add.cu
	nvcc $^ -o $@

clean:
	$(RM) *.o
	$(RM) crypto/*.o
	$(RM) util/*.o
	$(RM) step2
	$(RM) hello
	$(RM) vector_add
