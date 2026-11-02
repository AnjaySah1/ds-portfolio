Reflection: Portfolio 3A

For this part, I built a min-heap and a hash table from scratch. I did it in small steps and committed after each one. First the heap with insert, heapifyUp, peek, size, and isEmpty. Then extractMin and heapifyDown. Then the hash table with insert and find, then remove, then the load factor and rehash. After that came the full test file, a Makefile, and the README.

The heap was the more straightforward part. Once I had the index rules down (parent at (i - 1) / 2, left child at 2i + 1 and right child at 2i + 2), insert and extractMin just move one item up or down until  the order is right. My first versions did not compile because of small mistakes. I left out a closing parenthesis in the if statement in heapifyUp, and I spelled the function heapifUp in one place and heapifyUp in another. I did the same thing later with heapifyDown.

A failed compile also confused me once. The old program still ran, so I was reading old output and thought my changes did nothing. After that I used && so the program only runs when the compile works.

The hash table took more thought. Separate chaining was easy to follow, but rehash needed care. When the table grows I can't just copy the buckets over, because the bucket number depends on how many buckets there are, so every entry has to be put in again. I tested this with a table of 4 buckets. Three items stayed at 4 buckets, the 4th item grew it to 8, and all four keys were still found afterward.

For the string hash I start at 5381 and do hash * 131 + character for each character. I used 131 instead of the usual 33 because my table starts at 8 buckets and doubles. With 8, 16, or 32 buckets, multiplying by 33 acts like multiplying by 1 once I take the remainder, so anagrams can land in the same bucket.

The test file has 79 checks. One check initially failed "both drain as 1..20", and the bug was in my test, not the heap. I had written != 1 where it should have been != i. Another time the program crashed with an underflow_error because I had left out the loop that fills the heap before the duplicates test.

I also made a mistake with .gitignore. I used paths that started with Portfolio3A/ from inside that folder, so they matched nothing. Changing them to /portfolio3a and /Portfolio3A fixed it.

In the end all 79 checks pass, it compiles with no warnings, and a fresh clone from GitHub builds and runs. Testing rehasing with only four buckets and checking every key right after the table grew was especially useful. It let me check that the entries moved to the correct buckets without having to inspect a large table.Next time, I would write a small test like this for any structure that resizes before moving on to larger tests.
