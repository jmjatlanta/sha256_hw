CXX=../../sycl/build/bin/clang++
CXXFLAGS=-I. -O2 -I../../sycl/include

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@


step2: main.o crypto/sha256.o uint256.o util/strencodings.o
	$(CXX) $(LFLAGS) -o $@ $^

hello: hello.cpp
	$(CXX) $(LFLAGS) -o $@ $^

clean:
	rm *.o
	rm crypto/*.o
	rm util/*.o
	rm step2
