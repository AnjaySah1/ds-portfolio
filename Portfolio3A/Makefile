CXX           = g++
CXXFLAGS   = -std=c++11 -Wall -Wextra -pedantic -g

portfolio3a: main.cpp MinHeap.h HashTable.h
	$(CXX) $(CXXFLAGS) -o portfolio3a main.cpp

run: portfolio3a
	./portfolio3a

clean:
	rm -f portfolio3a

.PHONY: run clean

