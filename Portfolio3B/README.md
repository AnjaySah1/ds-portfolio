# Portfolio 3B: Priority Task Scheduler and Word Frequency Counter

This part uses the MinHeap and HashTable from Portfolio 3A for a task scheduler and a word frequency counter. Both are checked against the C++ standard library. One program, `portfolio3b`, runs both applications and the tests.

## Task scheduler

Each task has a name, a priority, and an insertion-order number. A lower priority number runs first, so priority 1 runs before priority 5. Tasks with the same priority run in the order they were added. The scheduler stores tasks in my MinHeap from Portfolio 3A.

Commands:

- `ADD <name> <priority>` adds a task.
- `NEXT` removes and shows the most urgent task.
- `LIST` shows waiting tasks in execution order without removing them.
- `CHECK` compares the waiting order against `std::priority_queue`.
- `DELAY <name> <newPriority>` changes a waiting task's priority.
- `QUIT` exits and prints the STL check summary.

Names can contain spaces. The last word in an `ADD` or `DELAY` command is the integer priority, and negative priorities are allowed. For example:

```text
ADD fix bug 1
ADD finish homework 7 2
```

The second command adds a task named `finish homework 7` with priority 2.

`LIST` copies the heap and takes tasks out of the copy, because the heap's array is not fully sorted. The original is left unchanged.

The insertion-order number breaks priority ties. Without one shared tie rule, my heap and the STL queue could both be correct and still give different orders.

## Word frequency counter

The counter splits a text file into lowercase words. Letters form words, and an apostrophe inside a word is kept, so "don't" is one word. Digits and other punctuation separate words. Leading and trailing apostrophes are dropped.

Words are counted in my HashTable. For each word, the counter looks it up first. If it is there, the count goes up by one. Otherwise it is inserted with a count of one. This matters because `HashTable::insert` replaces the value of an existing key.

The HashTable can't list its keys, so the counter also keeps a vector of the different words in the order they first appear.

### Top 10

The top 10 uses my MinHeap to keep only the best ten words seen so far. Each distinct word and its count is inserted, and if the heap grows past ten entries, the worst one is removed.

The word with the lowest count is at the root. For equal counts, the alphabetically later word counts as worse, so the alphabetically earlier words are kept when several words tie at the cutoff. The entries come out from worst to best and are reversed for display. The report shows the highest count first, with ties in alphabetical order. If there are fewer than ten different words, all of them are shown.

## STL cross-checks

**Scheduler against `std::priority_queue`.** Every task is added to both. Each `NEXT` compares the two results and prints `[STL check: PASS]` or `[STL check: FAIL]`. `CHECK` compares the whole remaining order using copies, without removing anything. When the program ends it prints a summary with the number of `NEXT` results checked, the mismatches, and an overall PASS or FAIL.

**Word counts against `std::unordered_map`.** The same words are counted again in an `unordered_map`. The program compares the total number of words, the number of different words, every word's count (checked in both directions, so a missing or extra word is caught), and the top 10 sorted with the same tie rule. It lists up to ten discrepancies, then prints PASS or FAIL.

## Bonus: DELAY

`DELAY <name> <newPriority>` changes a waiting task's priority. A smaller number makes it more urgent, and a larger number makes it less urgent.

If several waiting tasks have the same name, the one that would run first is changed. The task keeps its original insertion-order number, so it still wins ties against tasks added after it.

### Why updating an array-based heap takes more work

Updating a priority in an array-based heap takes more work because it takes two steps: find the task, then fix the heap order.

A heap is ordered by priority, not by name. Finding a task by name can mean checking all n items, which is O(n). Once the task's position in the array is known, a smaller priority number moves it up with `heapifyUp`, and a larger one moves it down with `heapifyDown`. That repair is O(log n).

To make the whole update O(log n), the heap would need a second structure that remembers where each task sits in the array, and every swap would have to update it. Names alone could not do this, because this scheduler allows duplicate names, so each task would need a unique ID.

### My approach

`Scheduler::delay` takes the tasks out in execution order, changes the chosen task, and builds a new heap from them. This is O(n log n), but it leaves the MinHeap from Portfolio 3A unchanged. `std::priority_queue` has no change-priority operation either, so the reference queue is rebuilt the same way, and the cross-check keeps working.

## Compile and run on ludwig.mcs.uvawise.edu

Portfolio3B uses `MinHeap.h` and `HashTable.h` from `../Portfolio3A`, so both folders must stay together in the repository. From the `Portfolio3B` folder:

```bash
make                             # build
./portfolio3b                    # interactive scheduler
./portfolio3b words sample.txt   # word frequency report for a file
./portfolio3b test               # self-tests
make test                        # self-tests, word report, and scheduler script
make clean                       # remove the program
```

To run the scheduler on a file of commands:

```bash
./portfolio3b < tasks.txt
```

It compiles with `g++ -std=c++11 -Wall -Wextra -pedantic` and gives no warnings.

## Tests

`./portfolio3b test` prints PASS or FAIL for every check and exits with status 1 if any check fails. The checks cover:

- an empty scheduler
- priority order, and arrival order for equal priorities (using names that are not in alphabetical order)
- `LIST` leaving the scheduler unchanged
- `DELAY` moving a task up or down, an unknown name changing nothing, and a delayed task keeping its place among equal priorities
- 4,000 random `ADD`, `DELAY`, and `NEXT` commands checked against `std::priority_queue`
- the tokenizer: lowercase, apostrophes, digits, and empty text
- an empty counter, repeated words, and a word that was never counted
- top-word order, alphabetical ties, and fewer than ten words
- 3,000 different words checked against `std::unordered_map` (this makes the HashTable rehash)
- deliberately wrong counts and extra words, to confirm that the cross-check reports FAIL

## Files

- `Scheduler.h`: Task and Scheduler
- `WordFreq.h`: tokenizer, word counter, top words, and the STL comparison
- `Tests.h`: the self-tests
- `main.cpp`: scheduler, word counter, and test modes
- `sample.txt` and `tasks.txt`: sample inputs
- `Makefile`

## Known limitations

- The tokenizer is intended for English text and does not support Unicode words.
- Only the plain apostrophe `'` is recognized. Curly apostrophes separate words.
- The whole file and its words are held in memory, so very large files use a lot of memory.
- Tasks exist only while the program runs. They are not saved.
- `LIST` and `DELAY` each take O(n log n) time.
- Only ten words are shown. If several words tie at the cutoff, the alphabetically earlier ones are kept.
