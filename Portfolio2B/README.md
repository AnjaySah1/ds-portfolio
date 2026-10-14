# Portfolio 2B: Contact Directory (BST-Backed)

## What I Built

A contact directory that stores `Contact` records containing a first name, last name, and phone number in my templated binary search tree (BST) from Portfolio 2A.

Contacts are ordered by last name, with first name as the tie-breaker, using `operator<`. The phone number is not part of the ordering, so contacts with the same first and last names are treated as duplicates.

The BST supports recursive insertion, searching, deletion, in-order traversal, size, and height. It also includes a pruning tree walk used by the `LOOKUP` , `DELETE` , and `PREFIX` commands.

## Commands

Command words are case-insensitive (`add` and `ADD` both work) and are entered one per line.

* `ADD <first> <last> <phone>` : Adds a contact and rejects duplicates.
* `LOOKUP <last>` : Displays every contact with the specified last name.
* `DELETE <last>` : Deletes all contacts with the specified last name.
* `DELETE <last> <first>` : Deletes the contact matching both names.
* `PREFIX <text>` : Displays contacts whose last names start with the specified text (BONUS)
* `LIST`: Displays all contacts in sorted order.
* `COUNT`: Reports the number of contacts and the tree height.
* `QUIT`: Exits the program.

`LOOKUP` and `DELETE` print a message when no matching contacts are found.
`LOOKUP`, `PREFIX`, and `DELETE` use the pruning tree walk to skip subtrees that cannot contain matching contacts.

## Files

* `BST.h`: Templated binary search tree with recursive operations, traversal methods, height and size calculations, deep copying, and the pruning walk.
* `Contact.h`: Contact structure and comparison operator.
* `main.cpp` : Command-line interface and command handlers.
* `Makefile` : Build, run, test, and clean rules.
* `tests.txt`: Commands used by `make test`.
* `expected_output.txt`: Expected output used to check the tests.

## How to Compile and Run

On `ludwig.mcs.uvawise.edu`, run:

```
make
make run
```
To run the automated tests:

```
make test
```

To remove generated build files:

```
make clean
```

The project uses `g++` with the `-Wall -Wextra -std=c++17` compiler flags and compiles with no warnings. The program can also read commands from a file:

```
./main < tests.txt
```

## Design Choices

* The spec says `DELETE <last>`, but two contacts can share a last name. I made `DELETE <last>` remove all of them, and `DELETE <last> <first>` remove just one.

## Known Limitations

* Names are case-sensitive, so `Anjay` and `anjay` are treated as different last names.
* Names and phone numbers cannot contain spaces because the driver reads each field separately.
* The BST does not balance itself. Inserting contacts in sorted order can produce a long chain, increasing operation times and potentially causing deep recursion for large inputs.


Tree height in COUNT counts nodes on the longest path from the root, so an empty directory shows height 0 and a single contact shows height 1.
