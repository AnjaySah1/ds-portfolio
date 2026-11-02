Reflection: Portfolio 3B

For this part, I built a priority task scheduler and a word frequency counter using my MinHeap and HashTable from Portfolio 3A. I developed the features in stages, compiled and tested them on Ludwig, fixed errors, and committed my progress along the way. This helped me understand how the data structures work in complete applications.

I started with the scheduler and then added the STL comparison, word counter, top 10, self-tests, and DELAY bonus. Working on one feature at a time made it easier to find problems.

Several compilation errors came from typing mistakes. I wrote #include <"Scheduler.h"> instead of using only quotes, mixed single and double quotes, and used different capitalization in nextMismatches. I also typed ./portolio3b in the Makefile. These mistakes taught me to read the first compiler error carefully and check spelling before making larger changes.

The word counter showed me why the input file matters. My report showed 74 words and 38 distinct words instead of the expected 75 and 37. My sample.txt had "samllest" instead of "smallest" and was missing "and". The program was correctly counting a different version of the sample. Another failure came from an extra block in `Tests.h` that expected the wrong task names. Looking at the numbered source lines helped me find and remove it. Next time, when a test fails, I would check the test itself before suspecting the data structure, because the leftover block was wrong and the scheduler was fine.

The scheduler uses an insertion-order number so tasks with equal priorities run in arrival order. Using the same rule in std::priority_queue makes the comparison consistent. For LIST, I extracted tasks from a copy of the heap so the original scheduler stayed unchanged.

For the word counter, I looked up each word before updating its count because HashTable::insert replaces an existing value. I also kept a vector of distinct words because my table has no function for listing its keys. The top-10 heap keeps only ten entries, removing the least desirable word when it grows beyond that limit. Alphabetical order breaks equal-frequency ties.

For DELAY, my MinHeap has no public function to find a task by name or update its priority. The scheduler extracts the tasks, changes the selected task, and rebuilds the heap. This takes O(n log n), but lets me reuse the Part 3A heap unchanged. A faster version would need to track each task's array position and update that information after swaps.

The final version passed all 52 self-tests, including random scheduler operations and tests that deliberately give the word checker incorrect counts. Both applications passed their STL comparisons. Finally, I tested a fresh GitHub clone on Ludwig, where both parts compiled without warnings and passed their tests.
