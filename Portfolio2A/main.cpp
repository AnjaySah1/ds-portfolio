#include "BST.h"
#include <iostream>
#include <string>

//Prints a vector on one line, like : 20 30 40

template <typename T>
void printVector(const std::vector<T>& v) {
	for (const T& item : v) {
	     std::cout << item << " ";
	}
	std::cout << std::endl;
}

int main() {

	// Test on an empty tree first
	BST<int> empty;
	std::cout << "Empty tree search(5): " << (empty.search(5) ? "true" : "false") << std::endl;
	std::cout << "Empty tree inorder: ";
	printVector(empty.inorder());

	// Test insert, search , and inorder with ints
	BST<int> tree;
	int values[] = {50, 30, 70, 20, 40, 60, 80};
	for (int v : values) {
	    tree.insert(v);
	}
	std::cout << "Inorder after inserting 7 values: ";
	printVector(tree.inorder());

	//Test Level-order traversal.
	std::cout << "Level Order traversal: ";
	printVector(tree.levelorder());

	std::cout << "search(40): " << (tree.search(40) ? "true" : "false") << std::endl;
    std::cout << "search(50) (the root): " << (tree.search(50) ? "true" : "false") << std::endl;
	std::cout << "search(99): " << (tree.search(99) ? "true" : "false") << std::endl;

	// Inserting a duplicate should not change the tree
	tree.insert(50);
	std::cout << "Inorder after inserting duplicate 50: ";
	printVector(tree.inorder());

	// Test with strings to make make sure the template works for another type
	BST<std::string> words;
	words.insert("pear");
	words.insert("apple");
	words.insert("zebra");
	words.insert("mango");
	std::cout << "String tree inorder: ";
	printVector(words.inorder());
	std::cout << "search(\"mango\"): " << (words.search("mango") ? "true" : "false") << std::endl;
	std::cout << "search(\"kiwi\"): " << (words.search("kiwi") ? "true" : "false") << std::endl;

	// Test removal of a leaf node.
	tree.remove(20);
	std::cout << "After removing leaf 20: ";
	printVector(tree.inorder());

	// Test removal of a node with two children.
	tree.remove(50);
	std::cout << "After remove node 50: ";
	printVector(tree.inorder());

	//Test removal of a missing value.
	tree.remove(999);
        std::cout << "After removing missing value: ";
        printVector(tree.inorder());

	// Test removal of the root from a single-node tree.
	BST<int> single;
	single.insert(10);
	single.remove(10);

	std::cout << "Single-node tree size after removal: "
		  << single.size() << std::endl;

	BST<int> oneChild;
	oneChild.insert(50);
	oneChild.insert(30);
	oneChild.insert(20);

	oneChild.remove(30);

	std::cout << "After removing node 30 with one child: ";
	printVector(oneChild.inorder());

	std::cout << "Size after one child removal: " << oneChild.size() << std::endl;



	return 0;
}
