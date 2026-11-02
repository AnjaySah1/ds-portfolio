// main.cpp
// Command loop for the priority task scheduler.
//	ADD <name> <priority>   add a task (name can have spaces)
//	NEXT			remove and show the most urgent task
//	LIST			show all tasks in order, without removing them
//	QUIT			exit

// Driver for Portfolio 3B.
//  ./portfolio3b               priority task scheduler (type commands)
//  ./portfolio3b words <file>  word frequency counter

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Scheduler.h"
#include "WordFreq.h"

// -----------------------------------------------------
// Task scheduler
// ----------------------------------------------------

static void printTask(const Task& t) {
    std::cout << t.name << " (priority " << t.priority << ")";
}

static void handleAdd(Scheduler& s, std::istringstream& in) {
     std::vector<std::string> words;
     std::string w;
     while (in >> w) {
	words.push_back(w);
     }
     if (words.size() < 2) {
	std::cout << "Usage: ADD <name> <priority>\n";
	return;
     }

     std::istringstream num(words.back());
     int priority = 0;
     char extra;
     if (!(num >> priority) || (num >> extra)) {
	std::cout << "Priority must be a whole number, like: ADD fix bug 1\n";
	return;
     }

     std::string name = words[0];
     for (std::size_t i = 1; i + 1 < words.size(); ++i) {
	 name += " " + words[i];
     }
     s.add(name, priority);
     std::cout << "Added: " << name << " (priority " << priority << ")\n";
}

static void handleNext(Scheduler& s) {
     Task t;
     if (s.next(t)) {
	std::cout << "Next task: ";
	printTask(t);
	std::cout << " [STL check: " << (s.lastNextMatched() ? "PASS" : "FAIL") << "]\n";
     } else {
	std::cout << "No tasks waiting.\n";
     }
}

static void handleCheck(const Scheduler& s) {
    std::cout << "Order of " << s.size() << " waiting task(s) vs std::priority_queue: "
	      << (s.matchesStl() ? "PASS" : "FAIL") << "\n";
}

static void printSummary(const Scheduler& s) {
    std::cout << "\nSTL cross-check summary\n";
    std::cout << " NEXT result checked: " << s.nextChecks() << "\n";
    std::cout << " Mismatches:          " << s.nextMismatches() << "\n";
    std::cout << " Waiting tasks order: " << (s.matchesStl() ? "PASS" : "FAIL") << "\n";
    bool ok = (s.nextMismatches() == 0 && s.matchesStl());
    std::cout << "  Overall:              " << (ok ? "PASS" : "FAIL") << "\n";
}

static void handleList(const Scheduler& s) {
     std::vector<Task> tasks = s.list();
     if (tasks.empty()) {
	std::cout << "No tasks waiting.\n";
	return;
     }
     std::cout << tasks.size() << " task(s) waiting, in order:\n";
     for (std::size_t i = 0; i < tasks.size(); ++i) {
	std::cout << " " << (i + 1) << ". " ;
	printTask(tasks[i]);
	std::cout << "\n";
     }
}

static void runScheduler() {
    Scheduler scheduler;
    std::cout << "Task scheduler. Commands: ADD <name> <priority>, NEXT, LIST, QUIT\n";

    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
	std::istringstream in(line);
	std::string command;
	in >> command;

	if (command == "ADD") {
	    handleAdd(scheduler, in);
	} else if (command == "NEXT") {
	    handleNext(scheduler);
	} else if (command == "LIST") {
	    handleList(scheduler);
	} else if (command == "CHECK") {
	    handleCheck(scheduler);
	} else if (command == "QUIT") {
	    break;
	} else if (!command.empty()) {
	    std::cout << "Unknown command: " << command << "\n";
	}
	std::cout << "> ";
     }
     printSummary(scheduler);
     std::cout << "Bye.\n";
}

// --------Word Frequency Counter--------------

static int runWords(const std::string& path) {
    std::string text;
    if (!readFile(path, text)) {
	std::cout << "Could not open file: " << path << "\n";
	return 1;
    }

    std::vector<std::string> tokens = tokenize(text);
    WordCounter counter;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
	counter.add(tokens[i]);
    }

    std::cout << "Word frequency report: " << path << "\n";
    std::cout << "  Total words:   " << counter.totalWords() << "\n";
    std::cout << "  Unique words:  " << counter.uniqueWords() << "\n";
    std::cout << "  Hash table:    " << counter.bucketCount() << " buckets, load factor "
	      << counter.loadFactor() << ", longest chain " << counter.longestChain() << "\n";

    std::vector<WordCount> top = counter.topWords(10);
    std::cout << "\nTop " << top.size() << " words (ties are listed alphabetically)\n";
    std::cout << "  Rank  Word             Count\n";
    for (std::size_t i = 0; i < top.size(); ++i) {
	std::cout << "  " << std::setw(4) << (i + 1) << "  " << std::left
		  << std::setw(15) << top[i].word << std::right
		  << std::setw(5) << top[i].count << "\n";
    }

    std::cout << "\nSTL cross-check (std::unordered_map)\n";
    bool ok = crossCheckWords(counter, tokens, std::cout);
    return ok ? 0 : 1;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
	runScheduler();
	return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "words") {
	return runWords(argv[2]);
    }
    std::cout << "Usage:\n"
	      << " " << argv[0] << "		     task scheduler (type commands)\n"
	      << " " << argv[0] << " words <file>    word frequency counter\n";
    return 1;
}
