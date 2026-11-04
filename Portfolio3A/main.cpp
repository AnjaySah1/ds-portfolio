// Test driver for Portfolio 3A: MinHeap<T> and HashTable<K, V>.

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "MinHeap.h"
#include "HashTable.h"

static int passed = 0;
static int failed = 0;

static void check(const std::string& name, bool ok) {
    if (ok) {
	++passed;
	std::cout << " [PASS] " << name << "\n";
    } else {
	++failed;
	std::cout << " [FAIL] " << name << "\n";
    }
}
static void section(const std::string& title) {
    std::cout << "\n== " << title << " ==\n";
}

// A custom type so the heap is tested on more than ints and strings.
struct Task {
    int priority;
    std::string name;
    Task() : priority(0) {}
  Task(int p, const std::string& n) : priority(p), name(n) {}
    bool operator<(const Task& other) const { return priority < other.priority; }
};

// A deliberately terrible hash: everything lands in one bucket. Used to test chains (removing from head, middle, and tail).
struct ConstantHash {
    std::size_t operator()(const int&) const { return 0; }
};

static void testMinHeap() {
    section("MinHeap: empty heap");
    MinHeap<int> h;
    check("new heap isEmpty", h.isEmpty());
    check("new heap size is 0", h.size() == 0);

    bool threw = false;
    try { h.extractMin(); } catch (const std::underflow_error&) { threw = true; }
    check("extractMin on empty throws", threw);

    threw = false;
    try { h.peek(); } catch (const std::underflow_error&) { threw = true; }
    check("peek on empty throws", threw);

    section("MinHeap: insert, peek, size");
    h.insert(5);
    check("single insert: peek is 5", h.peek() == 5);
    h.insert(3);
    check("insert smaller: peek is 3", h.peek() == 3);
    h.insert(8);
    h.insert(1);
    h.insert(9);
    check("peek is 1 after more inserts", h.peek() == 1);
    check("size is 5", h.size() == 5);
    check("not empty", !h.isEmpty());
    check("peek does not remove", h.size() == 5 && h.peek() == 1);
    check("heap rule holds", h.isValid());

    section("MinHeap: extractMin order");
    std::vector<int> out;
    while (!h.isEmpty()) {
	out.push_back(h.extractMin());
	if (!h.isValid()) { out.push_back(-999); }
    }
    bool sorted = (out.size() == 5 && out[0] == 1 && out[1] == 3 &&
		   out[2] == 5 && out[3] == 8 && out[4] == 9);
    check("values come out 1 3 5 8 9", sorted);
    check("empty again after draining", h.isEmpty() && h.size() == 0);

    section("MinHeap: duplicates");
    MinHeap<int> d;
    int vals[] = {4, 2, 4, 2, 7, 7, 1, 1};
    for (int i = 0; i < 8; ++i) d.insert(vals[i]);
    bool ok = true;
    int prev = d.extractMin();
    while (!d.isEmpty()) {
	int cur = d.extractMin();
	if (cur < prev) ok = false;
	prev = cur;
    }
    check("duplicates come out in non-decreasing order", ok);

    section("MinHeap: sorted and reverse sorted input");
    MinHeap<int> a, b;
    for (int i = 1; i <= 20; ++i) a.insert(i);
    for (int i = 20; i >= 1; --i) b.insert(i);
    check("ascending input is valid", a.isValid());
    check("descending input is valid", b.isValid());
    bool both = true;
    for (int i = 1; i <= 20; ++i) {
	if (a.extractMin() != i || b.extractMin() != i) both = false;
    }
    check("both drain as 1..20", both);

    section("MinHeap: 500 pseudo-random values");
    std::srand(12345);
    MinHeap<int> big;
    for (int i = 0; i < 500; ++i) big.insert(std::rand() % 1000);
    check("size is 500", big.size() == 500);
    check("valid after 500 inserts", big.isValid());
    ok = true;
    int last = big.extractMin();
    int count = 1;
    while (!big.isEmpty()) {
	int cur = big.extractMin();
	if (cur < last) ok = false;
	last = cur;
	++count;
    }
    check("drained 500 values in sorted order", ok && count == 500);

    section("MinHeap: strings");
    MinHeap<std::string> s;
    s.insert("pear");
    s.insert("apple");
    s.insert("mango");
    s.insert("banana");
    check("peek is apple", s.peek() == "apple");
    check("extract apple", s.extractMin() == "apple");
    check("extract banana", s.extractMin() == "banana");
    check("extract mango", s.extractMin() == "mango");
    check("extract pear", s.extractMin() == "pear");

    section("MinHeap: custom struct (Task)");
    MinHeap<Task> t;
    t.insert(Task(3, "write report"));
    t.insert(Task(1, "fix bug"));
    t.insert(Task(2, "review code"));
    check("peek is priority 1", t.peek().priority == 1 && t.peek().name == "fix bug");
    check("first out is fix bug", t.extractMin().name == "fix bug");
    check("second out is review code", t.extractMin().name == "review code");
    check("third out is write report", t.extractMin().name == "write report");

    section("MinHeap: interleaved insert and extract");
    MinHeap<int> m;
    m.insert(10);
    m.insert(4);
    int x = m.extractMin();
    m.insert(2);
    m.insert(6);
    int y = m.extractMin();
    int z = m.extractMin();
    check("interleaved results 4, 2, 6", x == 4 && y == 2 && z == 6);
    check("one item left (10)", m.size() == 1 && m.peek() == 10);
}

static void testHashTable() {
    section("HashTable: empty table");
    HashTable<std::string, int> t(4);
    check("new table isEmpty", t.isEmpty());
    check("new table size is 0", t.size() == 0);
    check("find on empty returns nullptr", t.find("nothing") == nullptr);
    check("remove on empty returns false", !t.remove("removing"));

    section("HashTable: insert and find (string keys)");
    check("insert apple is new", t.insert("apple", 1));
    check("insert banana is new", t.insert("banana", 2));
    check("insert cherry is new", t.insert("cherry", 3));
    check("size is 3", t.size() == 3);
    check("find apple = 1", t.find("apple") && *t.find("apple") == 1);
    check("find banana = 2", t.find("banana") && *t.find("banana") == 2);
    check("find cherry = 3", t.find("cherry") && *t.find("cherry") == 3);
    check("find missing key is nullptr", t.find("durian") == nullptr);

    section("HashTable: updating an existing key");
    check("insert apple again returns false", !t.insert("apple", 100));
    check("size is still 3", t.size() == 3);
    check("apple now 100", *t.find("apple") == 100);

    section("HashTable: find lets you change the value");
    int* p = t.find("banana");
    if (p) *p = 222;
    check("banana changed to 222", *t.find("banana") == 222);

    section("HashTable: remove");
    check("remove banana returns true", t.remove("banana"));
    check("banana is gone", t.find("banana") == nullptr);
    check("size is 2", t.size() == 2);
    check("remove banana again returns false", !t.remove("banana"));
    check("other keys survive", t.find("apple") != nullptr && t.find("cherry") != nullptr);

    section("HashTable: rehash at the load factor threshold");
    HashTable<std::string, int> r(4);
    r.insert("a", 1);
    r.insert("b", 2);
    r.insert("c", 3);
    check("3 of 4 buckets (0.75) does not rehash", r.bucketCount() == 4);
    r.insert("d", 4);
    check("4th insert (1.0 > 0.75) doubles to 8", r.bucketCount() == 8);
    check("load factor now 0.5", r.loadFactor() == 0.5);
    check("all 4 keys survive the rehash",
          r.find("a") && r.find("b") && r.find("c") && r.find("d") &&
          *r.find("a") == 1 && *r.find("b") == 2 &&
          *r.find("c") == 3 && *r.find("d") == 4);

    section("HashTable: 1000 string keys, many rehashes");
    HashTable<std::string, int> big;
    for (int i = 0; i < 1000; ++i) {
	big.insert("key" + std::to_string(i), i);
    }
    check("size is 1000", big.size() == 1000);
    check("load factor stays at or below 0.75", big.loadFactor() <= 0.75);
    bool ok = true;
    for (int i = 0; i < 1000; ++i) {
	int* v = big.find("key" + std::to_string(i));
	if (!v || *v != i) ok = false;
    }
    check("all 1000 keys found with right values", ok);
    for (int i = 0; i < 1000; i += 2) big.remove("key" + std::to_string(i));
    check("size is 500 after removing evens", big.size() == 500);
    ok = true;
    for (int i = 0; i < 1000; ++i) {
	bool present = big.find("key" + std::to_string(i)) != nullptr;
	if (present != (i % 2 == 1)) ok = false;
    }
    check("only odd keys remain", ok);

    section("HashTable: int keys");
    HashTable<int, std::string> n;
    n.insert(42, "answer");
    n.insert(7, "lucky");
    n.insert(-5, "negative");
    check("find 42", n.find(42) && *n.find(42) == "answer");
    check("find -5", n.find(-5) && *n.find(-5) == "negative");
    check("remove 7", n.remove(7) && n.find(7) == nullptr);


    section("HashTable: all keys in one chain (ConstantHash)");
    HashTable<int, int, ConstantHash> c;
    for (int i = 1; i <= 5; ++i) c.insert(i, i * 10);
    check("size is 5", c.size() == 5);
    check("longest chain is 5", c.longestChain() == 5);
    check("remove from the middle (3)", c.remove(3) && c.find(3) == nullptr);
    check("remove from the head (1)", c.remove(1) && c.find(1) == nullptr);
    check("remove from the tail (5)", c.remove(5) && c.find(5) == nullptr);
    check("2 and 4 still there", c.find(2) && *c.find(2) == 20 &&
				 c.find(4) && *c.find(4) == 40);
    check("size is 2", c.size() == 2);

    section("HashTable: copying");
    HashTable<std::string, int> copy(t);
    copy.insert("zebra", 26);
    check("copy has original keys", copy.find("apple") && *copy.find("apple") == 100);
    check("copy has its new key", copy.find("zebra") != nullptr);
    check("original does not have the new key", t.find("zebra") == nullptr);
}

static void hashDemo() {
     section("String hash demo");
     CustomHash<std::string> h;
     const char* words[] = {"cat", "act", "tac", "Cat", "hello", ""};
     for (int i = 0; i < 6; ++i) {
	 std::string w = words[i];
	 std::cout << " hash(\"" << w << "\") = " << h(w)
		   << " bucket in a table of 16: " << h(w) % 16 << "\n";
	 }
	 check("anagrams cat/act/tac hash differently",
	       h("cat") != h("act") && h("act") != h("tac") && h("cat") != h("tac"));
	 check("same string always hashes the same", h("hello") == h("hello"));
	 check("case matters (cat vs Cat)", h("cat") != h("Cat"));
	 check("empty string hashes to the starting value 5381", h("") == 5381);

	 HashTable<std::string, int> spread;
	 for (int i = 0; i < 1000; ++i) spread.insert("word" + std::to_string(i), i);
	 std::cout << " 1000 keys: " << spread.bucketCount() << " buckets, longest chain "
		   << spread.longestChain() << "\n";
	 check("longest chain is small (10 or less)", spread.longestChain() <= 10);
}

int main() {
    std::cout << "Portfolio 3A: MinHeap and HashTable tests\n";
    testMinHeap();
    testHashTable();
    hashDemo();

    std::cout << "\n-------------------------------\n";
    std::cout << "Passed: " << passed << " Failed: " << failed << "\n";
    std::cout << (failed == 0 ? "All tests passed.\n" : "Some test FAILED.\n");
    return failed == 0 ? 0 : 1;
}

