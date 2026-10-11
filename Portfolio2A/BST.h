
#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include <queue>

// Templated Binary search Tree. Only needs operator< on T.
// Includes the Rule of Three (destructor, copy constructor, copy assignment) with a full recursive deep copy.


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

     // Here, walks down the tree to find the right spot for value and returns the root of that subtree. If the value is already there, nothing changes, so duplicates are ignored.

     Node* insertHelper(Node* node, const T& value)
     {
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

     bool searchHelper(const Node* node, const T& value) const
     {
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

     void destroyHelper(Node* node)
     {
        if (node == nullptr) {
            return;
        }
        destroyHelper(node->left);
        destroyHelper(node->right);
        delete node;
     }

     // Builds a brand new copy of a subtree( copy node, then both children).
     // The new tree shares no nodes with the original.

      Node* copyHelper(const Node* node) const
      {
        if (node == nullptr) {
                return nullptr;
        }
        Node* copy = new Node(node->value);
        copy->left = copyHelper(node->left);
        copy->right = copyHelper(node->right);
        return copy;
      }

    // Finds the smallest value in a subtree.
    // The inorder successor is the leftmost node in the right subtree.
     Node* minNode(Node* node)
     {
        while (node->left != nullptr) {
                node = node->left;
        }
        return node;
     }

     // Removes a value from a subtree and returns the updated subtree root.
     Node* removeHelper(Node* node, const T& value)
     {
        // Case: value is not in this subtree.
        if (node == nullptr) {
            return nullptr;
        }

        // Search the left subtree.
        if (value < node->value) {
                node->left = removeHelper(node->left, value);
        }

        // Search the right subtree.
        else if (node->value < value) {
                node->right = removeHelper(node->right, value);
        }
        // The current node contains the value to remove.
        else {
                // Case 1: no children (leaf).
                if (node->left == nullptr && node->right == nullptr) {
                        delete node;
                        return nullptr;
                }

                // Case 2a: only a right child.
                if (node->left == nullptr) {
                        Node* child = node->right;
                        delete node;
                        return child;
                }

                // Case 2b: only a left child.
                if (node->right == nullptr) {
                        Node* child = node->left;
                        delete node;
                        return child;
                }

                //Case 3: two children.
                // Replace this value with its inorder successor.The successor value is copied first so we never read it from a node that is about to be deleted.
                T successorValue = minNode(node->right)->value;
                node->value = successorValue;

                //Remove the original successor node.
                node->right = removeHelper(node->right, successorValue);
        }
        return node;
     }


     //Left then node then right (L N R): In a BST, inorder gives sorted order.

     void inorderHelper(const Node* node, std::vector<T>& result) const
     {
        if (node == nullptr) {
            return;
        }
        inorderHelper(node->left, result);
        result.push_back(node->value);
        inorderHelper(node->right, result);
     }

      // Counts this node plus everything in its left and right subtrees
      int sizeHelper(const Node* node) const
      {
        if (node == nullptr)  {
                return 0;
        }
        return 1 + sizeHelper(node->left) + sizeHelper(node->right);
      }

      // Height is the number of nodes on the longest path going down from here.
      // An empty tree is 0 and a single node is 1.

       int heightHelper(const Node* node) const
       {
         if (node == nullptr) {
                return 0;
         }
         int leftHeight = heightHelper(node->left);
         int rightHeight = heightHelper(node->right);
         return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        }

        //PreOrder: Node first then left then right ( N L R).
        void preorderHelper(const Node* node, std::vector<T>& result) const
        {
                if (node== nullptr)  {
                        return;
                }
                result.push_back(node->value);
                preorderHelper(node->left, result);
                preorderHelper(node->right, result);
        }

        //Postorder: Left then right then node last ( L R N)
        void postorderHelper (const Node* node, std::vector<T>& result) const
        {
                if (node == nullptr)  {
                        return;
                }
                postorderHelper(node->left, result);
                postorderHelper(node->right, result);
                result.push_back(node->value);
        }

public:
     BST() : root(nullptr) {}

    //Rule of Three,Part 1: destructor frees every node so there are no memory leaks
     ~BST() {
        destroyHelper(root);
     }

     // Rule of Three , Part 2: copy constructor makes a full deep copy.
     BST(const BST& other) : root(copyHelper(other.root))  {}

     // Rule of Three, Part 3: copy assignment. The copy is built first and the old tree is freed afterwards, and self assignment is safe.
     BST& operator=(const BST& other)
     {
        if (this != &other)  {
                Node* newRoot = copyHelper(other.root);
                destroyHelper(root);
                root = newRoot;
        }
        return *this;
      }

      void insert(const T& value)
      {
        root = insertHelper(root, value);
      }

      bool search(const T& value) const
      {
        return searchHelper(root, value);
      }

      void remove(const T& value)
      {
        root = removeHelper(root, value);
      }

      // Returns all the values in sorted order (using inorder)

      std::vector<T> inorder() const
      {
        std::vector<T> result;
        inorderHelper(root, result);
        return result;
      }
      int size() const
      {
        return sizeHelper(root);
      }

      int height() const
      {
        return heightHelper(root);
      }

      std::vector<T> preorder() const
      {
        std::vector<T> result;
        preorderHelper(root, result);
        return result;
      }

      std::vector<T> postorder() const
      {
        std::vector<T> result;
        postorderHelper(root, result);
        return result;
      }

      // Level by level, left to right , using a queue.
      std::vector<T> levelorder() const
      {
        std::vector<T> result;

        if (root == nullptr)
        {
                return result;
        }

        std::queue<const Node*> nodes;
        nodes.push(root);

        while (!nodes.empty())  {
                const Node* current = nodes.front();
                nodes.pop();

                result.push_back(current->value);
                if (current->left != nullptr) {
                        nodes.push(current->left);
                }

                if (current->right != nullptr)  {
                        nodes.push(current->right);
                }
        }
        return result;
     }
};  // End of class BST

#endif          // End of header guard
