# Reflection: Portfolio 2B, Contact Directory

For this part I used my own BST class from Part A to build a contact directory. Each contact has a first name, last name, and phone number. The tree sorts them by last name, and uses first name when the last names match. The commands are ADD, LOOKUP, DELETE, LIST, and COUNT, plus the PREFIX bonus.

The part that took the most thought was DELETE. The assignment says DELETE <last>, but my tree is keyed by last name and first name, so two people can share a last name.
I had to decide what the command should do in that case. I made DELETE <last> remove every contact with that last name, and DELETE <last> <first> remove just one. I wrote this choice in the README so it is clear it was on purpose. The removal itself uses the removeHelper I wrote in Part A, so the leaf, one-child, and two-child cases were already working.

The part I liked most was the pruning walk. I wrote one function, collect, that takes a comparison and skips any subtree that cannot match. LOOKUP, DELETE, and PREFIX all use it. Because the tree is sorted, it follows one path plus the matches instead of checking every contact. Using one function for all three commands kept main.cpp short.

Testing taught me the most. My first tests.txt only deleted the root over and over, so it did not really test DELETE. I added a contact so I could delete a leaf, a one-child node, and a two-child node that was not the root.
Then I drew the tree on paper and checked the output line by line before I saved it as expected_output.txt.

My mistakes in this part were small ones. When I added DELETE and PREFIX, I had a quote that opened with a double quote and closed with a single quote, and I typed txt instead of text in a lambda. The compiler pointed to both lines, and fixing them made the other errors go away.

If I kept working on this, I would add balancing because adding contacts in alphabetical order turns the tree into a long chain. I would also make names case-insensitive and allow spaces in them.

The main thing I take from this is how much a clear comparison function matters. Once operator< in Contact.h was right, the rest of the tree worked the way I expected.
