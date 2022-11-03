CXX=g++
CXXFLAGS=-I. -O2

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@


step1: main.o crypto/sha256.o uint256.o util/strencodings.o
	$(CXX) $(LFLAGS) -o $@ $^


clean:
	rm *.o
	rm crypto/*.o
	rm util/*.o
	rm step1
