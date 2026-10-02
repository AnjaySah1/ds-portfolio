#ifndef STACK_H
#define STACK_H

#include <stdexcept>

// Templated stack, backed by a linked list of nodes.

template <typename T>
class Stack {
private:
// Each node holds one value and a pointer to the node below it in the stack
    struct Node {
	T data;
	Node* next;
	Node(const T& value, Node* nextNode) : data(value), next(nextNode) {}
    };

    Node* topNode;  // points to the node currently on top
    int count;    // how many elements are in the stack

//Helper for the copy constructor and operator=
//Copies 'other' node by node so this stack ends up with its own
// Separate nodes instead of pointing at the same memory as 'other'
    void copyFrom(const Stack& other) {
	topNode = nullptr;
	count = 0;
	if (other.topNode == nullptr) {
	    return;
	}
// walk 'other' from top to bottom, building a temporary list.

	Node* otherCurrent = other.topNode;
	Node* reversedHead = nullptr;

	while (otherCurrent != nullptr) {
	    reversedHead = new Node(otherCurrent->data, reversedHead);
	    otherCurrent = otherCurrent->next;
	}
	// Push from the reversed (bottom -to- top) list so the final stacks ends up with the same top as 'other'

	Node* current = reversedHead;
	while (current != nullptr) {
	    push(current->data);
	    current = current->next;
	}
	// Free the temporary reversed list, it was only needed to get the order right
	while (reversedHead != nullptr) {
	    Node* toDelete = reversedHead;
	    reversedHead = reversedHead->next;
	    delete toDelete;
	}
    }
	// Frees every node in the stack. Used by the destructor and by operator= before it copies in new data
    void clear() {
	while (topNode != nullptr) {
	    Node* toDelete = topNode;
	    topNode = topNode->next;
	    delete toDelete;
	}
	count =0;
    }

public:
    Stack() : topNode(nullptr), count(0) {}
	// Destructor : It frees all nodes so we don't leak memory
    ~Stack() {
	clear();
    }
	// Copy constructor - deep copy of ' other'
    Stack(const Stack& other) {
	copyFrom(other);
    }
	// Copy assignment- deep copy with a self assignment guard( a = a)
    Stack& operator=(const Stack& other) {
	if (this == &other) {
	    return *this;
	    }
	    clear();
	    copyFrom(other);
	    return *this;
    }

	// Adds a new value to the top of the stack
    void push(const T& value)  {
	topNode = new Node(value, topNode);
	count++;
    }
	// Removes and returns the value on top of the stack.
	// Throws an exception if the stack is empty
    T pop()  {
	if (isEmpty())  {
	    throw std::runtime_error("pop() called on empty stack ");
	}
	Node* toDelete = topNode;
	T value = toDelete->data;
	topNode = topNode->next;
	delete toDelete;
	count--;
	return value;
    }

	// Returns a reference to the value on top, without removing it. Throws an exception if the stack is empty
    T& top()  {
	if (isEmpty()) {
	    throw std::runtime_error("top() called on empty stack ");
	}
	return topNode->data;
    }

	// const version of top() , for when the stack itself is const
    const T& top() const  {
	if (isEmpty())  {
	    throw std::runtime_error("top() called on empty stack ");
	}
	return topNode->data;
    }

    bool isEmpty() const { return topNode == nullptr; }
    int size() const {return count; }
};

#endif
