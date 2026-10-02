// Portfolio

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

// Templated singly linked list and works with any type T ( int, string, custom structs, etc.)

template <typename T>
class LinkedList {
private:
// Each node holds one value and a pointer to the next node in the list
  struct Node {
	T data;
	Node* next;
	Node(const T& value) : data(value), next(nullptr) {}
     };

     Node* head;   // Points to the first node in the list
     int count;    // Keeps track of how many elements are in the list

// Here Helper used by both the copy constructor and operator=
// Builds new nodes copied from "other" instead of copying pointers, so the lists do not end up sharing memory to each other.

     void copyFrom(const LinkedList& other) {
	  head = nullptr;
	  count = 0;
	  Node* otherCurrent = other.head;
	  while (otherCurrent != nullptr) {
		pushBack(otherCurrent->data);
		otherCurrent = otherCurrent->next;
	}
     }

// Helper that frees every node in the list which are used by destructor and by operator= before it copies in new data

     void clear() {
	  Node* current = head;
	  while (current != nullptr) {
		Node* toDelete = current;
		current = current->next;
		delete toDelete;
	  }
	  head = nullptr;
	  count = 0;
     }

public:
	LinkedList() : head(nullptr), count(0) {}

	// Destructor : It frees all nodes so we don't have leak memory
	~LinkedList()	{
		clear();
	}

	// Copy Constructor - makes a Deep copy of "other" ( new nodes, not shared pointers)
	LinkedList(const LinkedList& other) {
		copyFrom(other);
	}

	// Copy assignment operator - deep copy with self assignemnt guard
	LinkedList& operator=(const LinkedList& other) {
		if(this==&other)
		{
		    return *this;   // Guard gaianst self assignment (a = a)
		}
		clear();	  //Free what we currently own
		copyFrom(other);  //then deep-copy the other list's nodes
		return *this;
	}
	//Adds a new value to the front of the list (PushFront)
	void pushFront(const T& value) 
	{
		Node* newNode = new Node(value);
		newNode->next = head;
		head = newNode;
		count++;
	}
	// Adds a new value to the end of the list
	void pushBack(const T& value)
	{
		Node* newNode = new Node(value);
		if (head == nullptr)
		{
		   head = newNode;
		} else {
			Node* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = newNode;
		}
		count++;
	}

	//Remove the first element in the list. Returns false if the list was already empty
	bool popFront()  {
	     if (head == nullptr)  {
		return false;
	     }
	     Node* toDelete = head;
	     head = head->next;
	     delete toDelete;
	     count--;
	     return true;
	}
	// Fines and removes the first node matching "value". Returns false if not found
	bool remove(const T& value) {
		Node* current = head;
		Node* previous = nullptr;

		while (current != nullptr) {
			if (current->data == value) {
			   if(previous == nullptr) {
				head = current->next;  //removing the head
			   } else {
				previous->next = current->next;
			   }
			   delete current;
			   count--;
			   return true;
			}
			previous = current;
			current = current->next;
		}
		return false;  // return false if value not found
	}
	//Returns true if "value" exists anywhere in the list
	bool contains(const T& value) const {
		Node* current = head;
		while (current != nullptr) {
		   if (current->data == value)  {
			return true;
		   }
		   current = current->next;
		}
		return false;
	}

	int size() const  { return count; }
	bool isEmpty() const { return count == 0; }

	// Prints the whole list on one line, e.g [1 2 3 ]
	void print() const  {
		Node* current = head;
		std::cout << "[ ";
		while (current != nullptr)  {
		   std::cout << current->data << " ";
		   current = current->next;
		}
		std::cout << "]" << std::endl;
	}

	// Iterator support so this class works in a range based for loop
	class Iterator {
	private:
		Node* current;
	public:
		Iterator(Node* node) : current(node)  {}

		T& operator*() {
		    return current->data;
		}

		Iterator& operator++() {
		    current = current->next;
		    return *this;
		}

		bool operator !=(const Iterator& other)  const  {
		    return current != other.current;
		}
	};

	Iterator begin() const {return Iterator(head); }
	Iterator end()  const { return Iterator(nullptr); }
};
#endif
