// MinHeap.h
// A min-heap built on std::vector<T>.
// For index i: parent is (i - 1) / 2, children are 2i + 1 and 2i + 2.
// Rule: every parent is <= its children, so the smallest item is at index 0.
// T needs operator<.

#ifndef MINHEAP_H
#define MINHEAP_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class MinHeap {
public:
    // Added this at the end,then bubble up untill the heap rule holds.
    void insert(const T& value) {
        data_.push_back(value);
	heapifyUp(data_.size() - 1);
    }

    // Smallest value, without removing it.
    const T& peek() const {
	if (data_.empty()) {
	    throw std::underflow_error("MinHeap::peek on an empty heap");
	}
	return data_.front();
    }

    std::size_t size() const { return data_.size(); }
    bool isEmpty() const { return data_.empty(); }

private:
    std::vector<T> data_;

    // Swap with the parent while smaller than it.
    void heapifyUp(std::size_t i) {
	while (i > 0) {
	    std::size_t parent = (i - 1) / 2;
	    if (data_[i] < data_[parent]) {
		std::swap(data_[i], data_[parent]);
		i = parent;
	    } else {
		break;
	    }
	}
     }
};

#endif
