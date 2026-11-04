# Portfolio 3A: Min-Heap and HashTable

This part has two data structures I built from scratch: a min-heap and a hash table. Both are templates, so each one is just a header file. `main.cpp` tests them.

## MinHeap

`MinHeap<T>` stores its items in a `std::vector<T>`. The smallest item is always at index 0. For an item at index `i`, the parent is at `(i - 1) / 2` and the children are at `2i + 1` and `2i + 2`.

- `insert` puts the new item at the end and moves it up with `heapifyUp`.
- `extractMin` takes the item at the root, moves the last item to the root, and moves it down with `heapifyDown`.
- `peek`, `size`, and `isEmpty` do what the names say.
- `peek` and `extractMin` throw an error if the heap is empty.
- I also added `isValid`, which checks the heap order. I use it in the tests.

## HashTable

`HashTable<K, V>` uses separate chaining. It is a vector of buckets, and each bucket is a linked list (I used `std::list`) of key/value pairs.

- `insert` adds a pair. If the key is already there, it replaces the value.
- `find` gives back a pointer to the value, or `nullptr` if the key is not there.
- `remove` deletes a key and says if it was found.
- I also added `size`, `isEmpty`, `bucketCount`, `loadFactor`, and `longestChain`.

### Load factor and rehash

Load factor is the number of items divided by the number of buckets. The limit is 0.75. After an insert, if the load factor goes above 0.75, the table makes twice as many buckets and puts every item in again. Every item has to be put in again because the bucket number depends on how many buckets there are.

Example: a table with 4 buckets can hold 3 items (exactly 0.75). The 4th item makes it 1.0, so it grows to 8 buckets.

### Hash function

For strings, I start with 5381. Then for each character I do `hash = hash * 131 + character`. Since it multiplies each time, the order of the letters matters, so "cat" and "act" get different hashes.

I used 131 and not 33 (the number djb2 normally uses). My table starts at 8 buckets and doubles each time, so the bucket count is a power of two unless I pick a different starting size. With 33 and 32 buckets or fewer, anagrams like "cat", "act", and "tac" land in the same bucket, because 33 is one more than a power of two. With 131 they spread out. Different strings can still collide, and separate chaining is what handles that.

Integers just hash to themselves. Any other key type uses `std::hash`.

## How to compile on ludwig.mcs.uvawise.edu

	make
	./portfolio3a

Without make:

	g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o portfolio3a

`make clean` deletes the program. There are no warnings when it compiles.
`./portfolio3a` runs 79 checks and prints PASS or FAIL for each one.

## Known limitations

- The table only grows. It does not shrink when I remove items.
- A rehash goes through every item, so the one insert that triggers it is slow.
- A pointer from `find` stops being valid if that key is removed or if the table rehashes.
- `MinHeap` only uses `<`, so it can't take a custom comparison.
