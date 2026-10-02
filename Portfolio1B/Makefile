CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TARGET   = main
SRCS     = main.cpp
HEADERS  = Stack.h TextBuffer.h

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
