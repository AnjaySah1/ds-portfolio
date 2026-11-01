// WordFreq.h
// Word frequency counter built on my HashTable from Portfolio 3A.

#ifndef WORDFREQ_H
#define WORDFREQ_H

#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "HashTable.h"

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

