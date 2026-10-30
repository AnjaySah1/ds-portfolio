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


    HashTable<std::string, int> r(4);
    r.insert("a", 1);
    r.insert("b", 2);
    r.insert("c", 3);
    std::cout<< "buckets after 3 inserts: " << r.bucketCount() << "\n";
    r.insert("d", 4);
    std::cout << "buckets after 4 inserts: " << r.bucketCount() << "\n";
    std::cout << "load factor: " << r.loadFactor() << "\n";
    std::cout << "a b c d: " << *r.find("a") << " " << *r.find("b") << " "
	      << *r.find("c") << " " << *r.find("d") << "\n";


    std::cout << "heap valid? " << h.isValid() << "\n";
    std::cout << "longest chain: " << r.longestChain() << "\n";
    HashTable<int, std::string> n;
    n.insert(42, "answer");
    std::cout << "int key 42: " << *n.find(42) << "\n";


    return 0;

}
