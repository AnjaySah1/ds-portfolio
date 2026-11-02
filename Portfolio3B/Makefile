CXX	 = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g -I../Portfolio3A

portfolio3b: main.cpp Scheduler.h WordFreq.h Tests.h ../Portfolio3A/MinHeap.h ../Portfolio3A/HashTable.h
	$(CXX) $(CXXFLAGS) -o portfolio3b main.cpp

run: portfolio3b
	./portfolio3b

test: portfolio3b
	./portfolio3b test
	./portfolio3b words sample.txt
	./portfolio3b < tasks.txt

clean:
	rm -f portfolio3b

.PHONY: run test clean

