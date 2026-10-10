
#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>

// Templated Binary search Tree. Only needs operator< on T.

template <typename T>
class BST {
private:
     struct Node {
	T value;
	Node* left;
	Node* right;
	Node(const T& v) : value(v), left(nullptr), right(nullptr) {}
     };

     Node* root;

     // Here it walks down the tree to find the right spot for value and returns the root of that subtree. If the value is already there, nothing changes, so duplicates are ignored.

     Node* insertHelper(Node* node, const T& value) {
	if (node == nullptr) {
	    return new Node(value);
	}
	if (value < node->value) {
	    node->left = insertHelper(node->left, value);
	} else if (node->value < value) {
	    node->right = insertHelper(node->right, value);
	}
	return node;
     }

// Goes left if value is smaller, right if it is bigger. If we hit nullptr the value is not in the  tree. If neither < works, then it's equal.

     bool searchHelper(const Node* node, const T& value) const {
	if (node == nullptr) {
	    return false;
	}
	if (value < node->value) {
	    return searchHelper(node->left, value);
	}
	if (node->value < value) {
	    return searchHelper(node->right, value);
	}
	return true;
     }

//Deletes the children first and then the node itself. Doing it in this order means we never lose the pointers to the children before freeing them.

     void destroyHelper(Node* node) {
	if (node == nullptr) {
	    return;
	}
	destroyHelper(node->left);
	destroyHelper(node->right);
	delete node;
     }


//First: Left then node then right (L N R): In a BST: inorder gives sorted order.

     void inorderHelper(const Node* node, std::vector<T>& result) const {
	if (node == nullptr) {
	    return;
	}
	inorderHelper(node->left, result);
	result.push_back(node->value);
	inorderHelper(node->right, result);
     }

public:
     BST() : root(nullptr) {}

//Destructor frees every node so there are no memory leaks
     ~BST() {
	destroyHelper(root);
     }

     void insert(const T& value)  {
	root = insertHelper(root, value);
     }

     bool search(const T& value) const {
	return searchHelper(root, value);
     }

// Returns all the values in sorted order (using inorder)

     std::vector<T> inorder() const {
	std::vector<T> result;
	inorderHelper(root, result);
	return result;
     }
};

#endif

