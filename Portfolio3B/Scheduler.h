// Scheduler.h
// A priority task scheduler built on my MinHeap from Portfolio 3A.
// A lower priority number means more urgent, so priority 1 runs before 5.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstddef>
#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "MinHeap.h"

struct Task {
    std::string name;
    int priority;
    std::size_t order;      // 0, 1, 2... in the order tasks were added

    Task() : priority(0), order(0) {}
    Task(const std::string& n, int p, std::size_t o)
	: name(n), priority(p), order(o) {}

    // Lower priority number goes first. If two tasks have the same priority, the one added first goes first. 
    // This makes the order the same every time, which the STL cross-check needs later.

    bool operator<(const Task& other) const {
	if (priority != other.priority) {
	    return priority < other.priority;
	}
	return order < other.order;
    }

    // std::priority_queue with std::greater needs this to act as a min-queue.
    bool operator>(const Task& other) const { return other < *this; }

    bool operator ==(const Task& other) const {
	return name == other.name && priority == other.priority &&
	       order == other.order;
    }
};

class Scheduler {
public:
    Scheduler() : nextOrder_(0), nextChecks_(0), nextMismatches_(0) {}

    void add(const std::string& name, int priority) {
        Task t(name, priority, nextOrder_);
	++nextOrder_;
	heap_.insert(t);
	stl_.push(t);
    }

    // Takes the most urgent task out of the scheduler and puts it in result. Return false ( and changes nothing) if there are no tasks.
    bool next(Task& result) {
	if (heap_.isEmpty()) {
	    return false;
	}
	result = heap_.extractMin();
	Task expected = stl_.top();
	stl_.pop();

	++nextChecks_;
	lastMatched_ = (result == expected);
	if (!lastMatched_) {
	    ++nextMismatches_;
	}
	return true;
    }

    // Returns all tasks in the order they would run, without removing them. MinHeap can't be walked in order, so this copies the heap and empties the copy.
    std::vector<Task> list() const {
	MinHeap<Task> copy = heap_;
	std::vector<Task> tasks;
	while (!copy.isEmpty()) {
	    tasks.push_back(copy.extractMin());
	}
	return tasks;
     }

     // Checks the whole remaining order of my heao against a copy of the STL queue, without removing anything.
     bool matchesStl() const {
	std::vector<Task> mine = list();
	StlQueue copy  = stl_;
	if (mine.size() != copy.size()) {
	    return false;
	}
	for (std::size_t i = 0; i< mine.size(); ++i) {
	    if (!(mine[i] == copy.top())) {
		return false;
	    }
	    copy.pop();
	}
	return true;
      }

     // Did the most recent next() match the STL queue?
     bool lastNextMatched() const { return lastMatched_; }
     std::size_t nextChecks() const { return nextChecks_; }
     std::size_t nextMismatches() const { return nextMismatches_; }


     std::size_t size() const { return heap_.size(); }
     bool isEmpty() const { return heap_.isEmpty(); }


private:
    typedef std::priority_queue<Task, std::vector<Task>, std::greater<Task> > StlQueue;

    MinHeap<Task> heap_;
    StlQueue stl_;
    std::size_t nextOrder_;
    std::size_t nextChecks_;
    std::size_t nextMismatches_;
    bool lastMatched_ = true;

};

#endif
