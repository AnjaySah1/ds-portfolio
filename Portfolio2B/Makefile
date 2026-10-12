
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TARGET   = main
SRCS     = main.cpp
HEADERS  = BST.h Contact.h

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) < tests.txt > actual_output.txt
	diff expected_output.txt actual_output.txt && echo "All tests passed."

clean:
	rm -f $(TARGET) actual_output.txt

.PHONY: run test clean
