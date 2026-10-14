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
        std::cout << "Empty tree size: " << empty.size()
                  << ", height: " << empty.height() << std::endl;

        // Test insert, search , and inorder with ints
        BST<int> tree;
        int values[] = {50, 30, 70, 20, 40, 60, 80};
        for (int v : values) {
            tree.insert(v);
        }
        std::cout << "Inorder after inserting 7 values: ";
        printVector(tree.inorder());

        //Test the other traversals.
        std::cout << "Preorder traversal: ";
        printVector(tree.preorder());

        std::cout << "Postorder traversal: ";
        printVector(tree.postorder());

        std::cout << "Level Order traversal: ";
        printVector(tree.levelorder());

        // Test size and height.
        std::cout << "size: " << tree.size() << ", height: " << tree.height() << std::endl;

        std::cout << "search(40): " << (tree.search(40) ? "true" : "false") << std::endl;
        std::cout << "search(50) (the root): " << (tree.search(50) ? "true" : "false") << std::endl;
        std::cout << "search(99): " << (tree.search(99) ? "true" : "false") << std::endl;

        // Inserting a duplicate should not change the tree
        tree.insert(50);
        std::cout << "Inorder after inserting duplicate 50: ";
        printVector(tree.inorder());

        // Test with strings to make sure the template works for another type
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
        std::cout << "Single-node tree size after removal: " << single.size() << std::endl;

        BST<int> oneChild;
        oneChild.insert(50);
        oneChild.insert(30);
        oneChild.insert(20);
        oneChild.remove(30);

        std::cout << "After removing node 30 with one child: ";
        printVector(oneChild.inorder());

        std::cout << "Size after one child removal: " << oneChild.size() << std::endl;

        //Test removal of a node with only a right child.
        BST<int> rightChild;
        rightChild.insert(50);
        rightChild.insert(70);
        rightChild.insert(80);
        rightChild.remove(70);
        std::cout << "After removing node 70  with only a right child: ";
        printVector(rightChild.inorder());

        // Two children, successor is the right child itself. Expect preorder: 70 30 80
        BST<int> succDirect;
        for (int v : {50, 30, 70, 80}) {
            succDirect.insert(v);
        }
        succDirect.remove(50);
        std::cout << "Preorder after removing 50 (successor is the right child): ";
        printVector(succDirect.preorder());

        // Two children, successor is deep in the right subtree and has a right child.
        // Expect preorder: 60 30 70 65, levelorder: 60 30 70 65, height: 3
        BST<int> succDeep;
        for (int v : {50, 30, 70, 60, 65}) {
            succDeep.insert(v);
        }
        succDeep.remove(50);
        std::cout << "Preorder after removing 50 (successor has a right child): ";
        printVector(succDeep.preorder());
        std::cout << "Level order after the same removal: ";
        printVector(succDeep.levelorder());
        std::cout << "Height after the same removal: " << succDeep.height() << std::endl;

        // Root with only one child. Expect inorder: 30 and size 1
        BST<int> rootOneChild;
        rootOneChild.insert(50);
        rootOneChild.insert(30);
        rootOneChild.remove(50);
        std::cout << "After removing root that has one child: ";
        printVector(rootOneChild.inorder());
        std::cout << "Size after removing root with one child: " << rootOneChild.size() << std::endl;

        //Rule of Three: copy constructor makes an independent deep copy.
        BST<int> original;
        for (int v : values) {
                original.insert(v);
        }

        BST<int> copied(original);
        copied.remove(50);
        copied.insert(999);
        std::cout << "Original after editing copy: ";
        printVector(original.inorder());
        std::cout << "Copy after edits: ";
        printVector(copied.inorder());

        //Rule of Three: copy assignment replaces the old contents.
        BST<int> assigned;
        assigned.insert(1);
        assigned = original;
        assigned.remove(30);
        std::cout << "Assigned copy after removing 30: ";
        printVector(assigned.inorder());
        std::cout << "Original still: ";
        printVector(original.inorder());

        //Self-assignment must be safe(goes through a reference so that compilers do not warn about an obvious self-assign).
        BST<int>& alias = original;
        original = alias;
        std::cout << "After self-assignment: ";
        printVector(original.inorder());

        //Copying an empty tree should also work.
        BST<int> emptyCopy(empty);
        std::cout << "Copy of empty tree size: " << emptyCopy.size() << std::endl;

        return 0;
}
