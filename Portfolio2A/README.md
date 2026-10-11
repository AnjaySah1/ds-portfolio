# Portfolio 2A: Binary Search Tree

## What Was Built

I implemented a templated binary search tree, `BST<T>`, in `BST.h`. The class uses a private `Node` struct containing a `value` and `left` and `right` child pointers along with a root pointer.

The tree supports recursive insertion and search. Duplicate values are ignored. The `remove()` function handles all three cases: removing a leaf node, a node with one child, and a node with two children using the in-order successor.

The implementation includes four traversals (inorder, preorder, postorder, and levelorder) that return vectors of values. It also provides recursive `height()` and `size()` functions.

The Rule of Three is implemented through a destructor, copy constructor, and copy-assignment operator, with recursive deep copying. The `main.cpp` file tests the implementation with integer and string values.

## How to Compile and Run

On `ludwig.mcs.uvawise.edu`, run the following commands from the project directory:
 * `make` : builds the executable `main` using `g++`  with `-Wall -Wextra -std=c++17`.
 * `make run`: builds and runs the program.
 * `make clean` : removes the generated executable.

## Known Limitations

The tree is not self-balancing. Inserting values in sorted order can produce a highly unbalanced tree, making some operations linear in time and increasing the risk of stack overflow for very large trees.
The stored type must support `operator<` and be copyable. Move construction and move assignment are not implemented.
