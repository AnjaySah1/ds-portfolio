// HashTable.h
// Hash table using separate chaining: a vector of buckets, where each bucket is a linked list of key/value entries.

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstddef>
#include <functional>
#include <list>
#include <string>
#include <vector>

// Fallback for other key types: using the standard library's hash.
template <typename K>
struct CustomHash {
    std::size_t operator()(const K& key) const {
	return std::hash<K>()(key);
    }
};

// Custom string hash: start at 5381, then for each character do  hash = hash * 131  + character
// Every character changes the result and position matters, so "cat" and "act" hash differently. 131 is used instead of 33 because with 33, anagrams land in the same bucket when the bucket count is a power of two. Overflow just wraps around for unsigned types.

template<>
struct CustomHash<std::string> {
    std::size_t operator()(const std::string& key) const {
	std::size_t hash = 5381;
	for (std::size_t i = 0; i < key.size(); ++i) {
	    hash = hash * 131 + static_cast<unsigned char>(key[i]);
	}
	return hash;
    }
};

template <typename K, typename V, typename Hash = CustomHash<K> >
class HashTable {
public:
    explicit HashTable(std::size_t initialBuckets = 8)
	: buckets_(initialBuckets == 0 ? 1 : initialBuckets), count_(0) {}

    // Add a pair. If the key already exits, replace its value.
    // Returns true for a new key, false for an update.
    bool insert(const K& key, const V& value) {
	std::list<Entry>& bucket = buckets_[indexFor(key)];
	for (typename std::list<Entry>::iterator it = bucket.begin();
	    it != bucket.end(); ++it) {
	  if (it->key == key) {
	      it->value = value;
	      return false;
	  }
	}
	bucket.push_back(Entry(key, value));
	++count_;
	return true;
     }

     // Pointer to the value, or nullptr if the key is not in the table.
     V* find(const K& key) {
	 std::list<Entry>& bucket = buckets_[indexFor(key)];
	 for (typename std::list<Entry>::iterator it = bucket.begin();
	      it != bucket.end(); ++it) {
	    if (it->key == key) {
		return &(it->value);
	    }
	  }
	  return nullptr;
      }

      std::size_t size() const { return count_; }
      bool isEmpty() const { return count_ == 0; }
      std::size_t bucketCount() const { return buckets_.size(); }

private:
    struct Entry {
	K key;
	V value;
	Entry(const K& k, const V& v) : key(k), value(v) {}
     };

     std::vector<std::list<Entry> > buckets_;
     std::size_t count_;
     Hash hash_;

     std::size_t indexFor(const K& key) const {
	 return hash_(key) % buckets_.size();
     }
};

#endif

