// WordFreq.h
// Word frequency counter built on my HashTable from Portfolio 3A.

#ifndef WORDFREQ_H
#define WORDFREQ_H

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "HashTable.h"
#include "MinHeap.h"

// Splits text into lowercase words. A word is a run of letters.
// An apostrophe counts as part of a word when a letter comes right after it (so "don't" is one word). Digits and punctuation end a word.

inline std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> words;
    std::string current;
    for (std::size_t i = 0; i < text.size(); ++i) {
	unsigned char c = static_cast<unsigned char>(text[i]);
	if (std::isalpha(c)) {
	    current += static_cast<char>(std::tolower(c));
	} else if (c == '\'' && !current.empty() && i + 1 < text.size() &&
		   std::isalpha(static_cast<unsigned char>(text[i + 1]))) {
	    current += '\'';
	} else if (!current.empty()) {
	    words.push_back(current);
	    current.clear();
	}
     }
     if (!current.empty()) {
	words.push_back(current);
     }
     return words;
}

// Reads a whole file into contents. Return false if it can't be opened.
inline bool readFile(const std::string& path, std::string& contents) {
    std::ifstream in(path.c_str());
    if (!in) {
	return false;
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    contents = buffer.str();
    return true;
}

// A word and how many times it appeared. This is what the top-N heap holds.
// Here "smaller" means "worse": fewer appearances, or the same number of appearances but later in the alphabet. So the best word is the one with the highest count, and ties are listed alphabetically.
struct WordCount {
    std::string word;
    int count;

    WordCount() : count(0) {}
    WordCount(const std::string& w, int c) : word(w), count(c) {}

    bool operator<(const WordCount& other) const {
	if (count != other.count) {
	    return count < other.count;
	}
	return word > other.word;
    }
};

class WordCounter {
public:
    WordCounter() : total_(0) {}

    // Count one word. HashTable::insert replaces the value of an existing key, so look the word up first and only insert when it is new.
    void add(const std::string& word) {
	int* existing = counts_.find(word);
	if (existing != nullptr) {
	    ++(*existing);
	} else {
	    counts_.insert(word, 1);
	    words_.push_back(word);
	}
	++total_;
     }

     // How many times a word was counted (0 if never).
     int count(const std::string& word) {
	 int* found = counts_.find(word);
	 return found == nullptr ? 0 : *found;
     }

     // The n most common words, best first. Uses my MinHeap as a "keep the best n" filter: every word goes in, and whenever the heap holds more than n, the worst word is removed. What is left at the end are the best n, and they come out worst to best, so the list is reversed.
     std::vector<WordCount> topWords(std::size_t n) {
	 MinHeap<WordCount> heap;
	 for (std::size_t i = 0; i < words_.size(); ++i) {
	     heap.insert(WordCount(words_[i], count(words_[i])));
	     if (heap.size() > n) {
		 heap.extractMin();
	     }
	 }
	 std::vector<WordCount> result;
	 while (!heap.isEmpty()) {
		result.push_back(heap.extractMin());
	 }
	 std::reverse(result.begin(), result.end());
	 return result;
      }

     std::size_t totalWords() const { return total_; }
     std::size_t uniqueWords() const { return counts_.size(); }

     // Every different word, in the order it first appeared. The HashTable can't list its keys, so this list is kept on the side.
     const std::vector<std::string>& words() const { return words_; }

     std::size_t bucketCount() const { return counts_.bucketCount(); }
     double loadFactor() const { return counts_.loadFactor(); }
     std::size_t longestChain() const { return counts_.longestChain(); }

private:
    HashTable<std::string, int> counts_;
    std::vector<std::string> words_;
    std::size_t total_;
};

#endif

