#include <iostream>
#include "MinHeap.h"
#include "HashTable.h"

int main() {
    MinHeap<int> h;
    h.insert(5);
    h.insert(3);
    h.insert(8);
    h.insert(1);
    h.insert(9);
    std::cout << "size: " << h.size() << "\n";    // 4
    std::cout << "peak: " << h.peek() << "\n";    // 1
    while (!h.isEmpty()) {
	std::cout << h.extractMin() << " ";
    }

    std::cout << "\n";



    HashTable<std::string, int> t;
    t.insert("apple", 1);
    t.insert("banana", 2);
    t.insert("apple", 100);      // update, not a new key
    std::cout << "table size: " << t.size() << "\n";
    std::cout << "apple: " << *t.find("apple") << "\n";
    std::cout << "banana: " << *t.find("banana") << "\n";
    std::cout << "durian found? " << (t.find("durian") != nullptr) << "\n";

    std::cout << "\n";

    std::cout << "remove banana: " << t.remove("banana") << "\n";
    std::cout << "remove banana again: " << t.remove("banana") << "\n";
    std::cout << "table size: " << t.size() << "\n";
    std::cout << "banana found? " << (t.find("banana") != nullptr) << "\n";
    std::cout << "apple still there? " << (t.find("apple") != nullptr) << "\n";





    return 0;

}
