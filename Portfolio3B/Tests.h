// Tests.h
// Self-tests for Portfolio 3B. Run with:  ./portfolio3b test
// Each check prints PASS or FAIL, and the program exits with 1 if any fail.

#ifndef TESTS_H
#define TESTS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Scheduler.h"
#include "WordFreq.h"

class TestReport {
public:
    TestReport() : passed_(0), failed_(0) {}

    void section(const std::string& title) {
	std::cout << "\n=== " << title << " ===\n";
    }

    void check(const std::string& name, bool ok) {
	if (ok) {
	    ++passed_;
	    std::cout << "  [PASS] " << name << "\n";
	} else {
	    ++failed_;
	    std::cout << "  [FAIL] " << name << "\n";
	}
     }

     int passed() const { return passed_; }
     int failed() const { return failed_; }

private:
    int passed_;
    int failed_;
};

// A word made only of letters, different for every n (0 -> "a", 1 -> "b", 26 -> "ab", ...). Used to make many different words.
inline std::string letterWord(int n) {
    std::string w;
    do {
	w += static_cast<char>('a' + n % 26);
	n /= 26;
    } while (n > 0);
    return w;
}

inline bool sameWords(const std::vector<std::string>& a,
		      const std::vector<std::string>& b) {
    return a == b;
}

// ------------------------ Scheduler tests ---------------------------------
inline void testScheduler(TestReport& t) {
    t.section("Scheduler: empty");
    {
	Scheduler s;
	Task task;
	t.check("empty scheduler isEmpty", s.isEmpty());
	t.check("NEXT on empty returns false", !s.next(task));
	t.check("LIST on empty is empty", s.list().empty());
	t.check("empty scheduler matches STL", s.matchesStl());
    }

    t.section("Scheduler: priority order");
    {
	Scheduler s;
	s.add("c", 3);
	s.add("a", 1);
	s.add("b", 2);
	s.add("urgent", -5);
	Task task;
	bool ok = s.next(task) && task.name == "urgent";
	ok = ok && s.next(task) && task.name == "a";
	ok = ok && s.next(task) && task.name == "b";
	ok = ok && s.next(task) && task.name == "c";
	t.check("tasks come out lowest priority number first", ok);
	t.check("scheduler is empty afterward", s.isEmpty());
	t.check("every NEXT matched the STL queue",
		s.nextChecks() == 4 && s.nextMismatches() == 0);
     }

     t.section("Scheduler: ties go in the order added");
     {
	Scheduler s;
	s.add("zeta", 2);
	s.add("alpha", 2);
	s.add("beta", 2);
	Task task;
	bool ok = s.next(task) && task.name == "zeta";
	ok = ok && s.next(task) && task.name == "alpha";
	ok = ok && s.next(task) && task.name == "beta";
	t.check("equal priorities come out first-added first", ok);
      }

    t.section("Scheduler: LIST does not remove anything");
    {
	Scheduler s;
	s.add("w", 3);
	s.add("x", 1);
	s.add("y", 2);
	std::vector<Task> listed = s.list();
	t.check("LIST shows 3 tasks", listed.size() == 3);
	t.check("LIST leaves size at 3", s.size() == 3);
	bool ok = true;
	for (std::size_t i = 0; i < listed.size(); ++i) {
	    Task task;
	    if (!s.next(task) || !(task == listed[i])) {
		ok = false;
	    }
	}
	t.check("NEXT gives tasks in the same order LIST showed", ok);
     }

     t.section("Scheduler: DELAY");
     {
	Scheduler s;
	s.add("a", 1);
	s.add("b", 2);
	s.add("c", 3);
	int old = 0;
	t.check("DELAY of an unknown task returns false", !s.delay("nope", 1, old));
	t.check("a failed DELAY changes nothing", s.size() == 3 && s.matchesStl());

	t.check("DELAY c to 0 works", s.delay("c", 0, old) && old == 3);
	std::vector<Task> listed = s.list();
	t.check("c is now first, then a, then b",
		listed.size() == 3 && listed[0].name == "c" &&
		listed[1].name == "a" && listed[2].name == "b");
	t.check("size is unchanged after DELAY", s.size() == 3);
	t.check("order still matches STL after DELAY", s.matchesStl());

	t.check("DELAY a to 9 (less urgent) works", s.delay("a", 9, old) && old == 1);
	Task task;
	bool ok = s.next(task) && task.name == "c";
        ok = ok && s.next(task) && task.name == "b";
        ok = ok && s.next(task) && task.name == "a" && task.priority == 9;
        t.check("a now runs last, with its new priority", ok);
    }

    t.section("Scheduler: DELAY and ties");
    {
	Scheduler s;
	s.add("x", 5);
	s.add("y", 3);
	int old = 0;
	s.delay("x", 3, old);
	Task task;
	bool ok = s.next(task) && task.name == "x";
	ok = ok && s.next(task) && task.name == "y";
	t.check("a delayed task keeps its place among equal priorities", ok);

	Scheduler dup;
	dup.add("same", 4);
	dup.add("same", 2);
	dup.delay("same", 9, old);
	t.check("with duplicate names, the one that runs first is changed",
		old == 2 && dup.list().size() == 2 &&
		dup.list()[0].priority == 4 && dup.list()[1].priority == 9);
    }

    t.section("Scheduler: 4000 random ADD, DELAY, and NEXT commands vs STL");
    {
	Scheduler s;
	std::srand(2026);
	bool orderOk = true;
	int adds = 0;
	int nexts = 0;
	for (int i = 0; i < 4000; ++i) {
	    int pick = std::rand() % 100;
	    if (pick < 55) {
		s.add("task" + std::to_string(i), std::rand() % 10 - 3);
		++adds;
	    } else if (pick < 70) {
		int old = 0;
		s.delay("task" + std::to_string(std::rand() % (i + 1)),
			std::rand() % 10 - 3, old);
	    } else {
		Task task;
		if (s.next(task)) {
		    ++nexts;
		}
	    }
	    if (i % 200 == 0 && !s.matchesStl()) {
                orderOk = false;
            }
        }
        t.check("order matched STL at every checkpoint", orderOk);
        t.check("no NEXT result differed from std::priority_queue",
                s.nextChecks() == static_cast<std::size_t>(nexts) &&
                s.nextMismatches() == 0);
        t.check("remaining tasks match STL",
                s.matchesStl() &&
                s.size() == static_cast<std::size_t>(adds - nexts));
    }
}


//----------------Word counter tests---------------------------

inline void testWords(TestReport& t) {
    t.section("Tokenizer");
    {
        std::vector<std::string> a = tokenize("Hello, World! Don't stop.");
        std::vector<std::string> expectA;
        expectA.push_back("hello");
        expectA.push_back("world");
        expectA.push_back("don't");
        expectA.push_back("stop");
        t.check("lowercases and keeps apostrophes inside words", sameWords(a, expectA));

        std::vector<std::string> b = tokenize("'quoted' dogs'");
        std::vector<std::string> expectB;
        expectB.push_back("quoted");
        expectB.push_back("dogs");
        t.check("leading and trailing apostrophes are dropped", sameWords(b, expectB));

        std::vector<std::string> c = tokenize("abc123def 4x");
        std::vector<std::string> expectC;
        expectC.push_back("abc");
        expectC.push_back("def");
        expectC.push_back("x");
        t.check("digits end a word", sameWords(c, expectC));

        t.check("empty text gives no words", tokenize("").empty());
        t.check("only punctuation gives no words", tokenize("... !!! ,,, 123").empty());
    }

    t.section("WordCounter: empty");
    {
        WordCounter c;
        t.check("no words counted", c.totalWords() == 0);
        t.check("no different words", c.uniqueWords() == 0);
        t.check("different-word list is empty", c.words().empty());
        t.check("any word has count 0", c.count("anything") == 0);
        t.check("topWords(10) is empty", c.topWords(10).empty());
        std::vector<std::string> none;
        std::ostringstream quiet;
        t.check("empty counter matches an empty unordered_map",
                crossCheckWords(c, none, quiet));
    }

    t.section("WordCounter: counts");
    {
        WordCounter c;
        std::vector<std::string> tokens = tokenize("the cat the dog the");
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            c.add(tokens[i]);
        }
        t.check("the = 3", c.count("the") == 3);
        t.check("cat = 1", c.count("cat") == 1);
        t.check("missing word = 0", c.count("bird") == 0);
        t.check("5 words in total", c.totalWords() == 5);
        t.check("3 different words", c.uniqueWords() == 3);
        t.check("different-word list has 3 entries", c.words().size() == 3);
    }

    t.section("WordCounter: top words");
    {
        WordCounter c;
        std::vector<std::string> tokens = tokenize("b a c a b a");
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            c.add(tokens[i]);
        }
        std::vector<WordCount> top = c.topWords(10);
        t.check("asking for 10 gives all 3 words", top.size() == 3);
        t.check("most common first (a=3, b=2, c=1)",
                top.size() == 3 && top[0].word == "a" && top[0].count == 3 &&
                top[1].word == "b" && top[1].count == 2 &&
                top[2].word == "c" && top[2].count == 1);
        t.check("topWords(2) gives 2 words", c.topWords(2).size() == 2);
        t.check("topWords(0) gives none", c.topWords(0).empty());

        WordCounter tie;
        tie.add("pear");
        tie.add("apple");
        tie.add("mango");
        std::vector<WordCount> tied = tie.topWords(3);
        t.check("equal counts are listed alphabetically",
                tied.size() == 3 && tied[0].word == "apple" &&
                tied[1].word == "mango" && tied[2].word == "pear");
    }

    t.section("WordCounter: 3000 different words vs unordered_map");
    {
        std::vector<std::string> tokens;
        std::srand(7);
        for (int i = 0; i < 3000; ++i) {
            tokens.push_back(letterWord(i));
        }
        for (int i = 0; i < 20000; ++i) {
            tokens.push_back(letterWord(std::rand() % 3000));
        }
        WordCounter c;
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            c.add(tokens[i]);
        }
        std::ostringstream quiet;
        t.check("table grew and every count matches unordered_map",
                crossCheckWords(c, tokens, quiet));
        t.check("23000 words, 3000 different",
                c.totalWords() == 23000 && c.uniqueWords() == 3000);
    }

    t.section("Cross-check can fail (it catches wrong counts)");
    {
        std::vector<std::string> tokens = tokenize("the cat and the dog and the bird");
        WordCounter good;
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            good.add(tokens[i]);
        }
        std::ostringstream quiet1;
        t.check("correct counts give PASS", crossCheckWords(good, tokens, quiet1));

        WordCounter wrongCount;
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            wrongCount.add(tokens[i]);
        }
        wrongCount.add("the");   // one "the" too many
        std::ostringstream quiet2;
        t.check("a wrong count gives FAIL", !crossCheckWords(wrongCount, tokens, quiet2));
        t.check("the report names the word",
                quiet2.str().find("\"the\"") != std::string::npos);

        WordCounter extraWord;
        for (std::size_t i = 0; i < tokens.size(); ++i) {
            extraWord.add(tokens[i]);
        }
        extraWord.add("zebra");  // a word the map never saw
        std::ostringstream quiet3;
        t.check("an extra word gives FAIL", !crossCheckWords(extraWord, tokens, quiet3));
    }
}

// Runs everything. Returns 0 if all checks pass, 1 otherwise.
inline int runSelfTests() {
    TestReport report;
    std::cout << "Portfolio 3B self-tests\n";
    testScheduler(report);
    testWords(report);

    std::cout << "\n--------------------------------\n";
    std::cout << "Passed: " << report.passed() << "   Failed: " << report.failed() << "\n";
    std::cout << (report.failed() == 0 ? "All tests passed.\n" : "Some tests FAILED.\n");
    return report.failed() == 0 ? 0 : 1;
}

#endif

