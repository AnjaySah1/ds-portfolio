#include <iostream>
#include "MinHeap.h"

int main() {
    MinHeap<int> h;
    h.insert(5);
    h.insert(3);
    h.insert(8);
    h.insert(1);
    std::cout << "size: " << h.size() << "\n";    // 4
    std::cout << "peak: " << h.peek() << "\n";    // 1
    return 0;

}
