//Main.cpp file 
#include "LinkedList.h"
#include <iostream>
#include <string>

// Custom struct used to prove the LinkedList template works with mroe than just built in types. Needs operator= so linkedList's contains()/remove() can compare values, and operator << so print() can display it.

struct Point {
    int x, y;
    bool operator ==(const Point& other) const {
	return x == other.x && y == other.y;
    }
};

std::ostream& operator <<(std::ostream& os, const Point& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

int main() {
	// Test with int
	LinkedList<int> intList;
	intList.pushBack(1);
	intList.pushBack(2);
	intList.pushBack(3);
	std::cout << "After pushback 1,2,3: ";
	intList.print();

	// Test pushfront
	intList.pushFront(0);
	std::cout << " After pushfront 0: ";
	intList.print();

	// test size and isEmpty
	std::cout << "Size: " << intList.size() << std::endl;
	std::cout << "isEmpty: " << (intList.isEmpty() ? "true" : "false") <<std::endl;

	//Test Contains
	std::cout << "contains(2): " << (intList.contains(2) ? "true" : "false") << std::endl;
	std::cout << "contains(99): " << (intList.contains(99) ? "true" : "false") <<std::endl;

	//Test remove (middle element, and a value not present)
	bool removed = intList.remove(2);
	std::cout << "remove(2) returned: " << (removed ? "true" : "false") << ", list is now: ";
	intList.print();
	bool removedMissing = intList.remove(99);
	std::cout << "remove(99) returned: " << (removedMissing ? "true" : "false") << std::endl;

	//Test popFront
	bool popped = intList.popFront();
	std::cout << "popFront() returned: " << (popped ? " true" : "false") << ", list is now: ";
	intList.print();
	std::cout << "size after popFront: " << intList.size() << std::endl;

	//Test popFront/contains/remove on an empty list
	LinkedList<int> emptyList;
	std::cout << "Empty list isEmpty(): " << (emptyList.isEmpty() ? "true" : "false") << std::endl;
	std::cout << "Empty list popFront() returned: " << (emptyList.popFront() ? "true" : "false") << std::endl;
	std::cout << "Empty list contains(5): " << (emptyList.contains(5) ? "true" : "false") << std::endl;
	std::cout << "Empty list remove(5) returned: " << (emptyList.remove(5) ? "true" : "false") << std::endl;

	//Test with string
	LinkedList<std::string> stringList;
	stringList.pushFront("world");
	stringList.pushFront("hello");
	stringList.print();
	std::cout << "contains(\"world\"): " << (stringList.contains("world") ? "true" : "false") << std::endl;
	stringList.remove("hello");
	std::cout << "After remove(\"hello\"): ";
	stringList.print();

	// Test with custom struct (point)
	LinkedList<Point> pointList;
	pointList.pushBack({1, 2});
	pointList.pushBack({3, 4});
	pointList.print();
	std::cout << "contains({1, 2}): " << (pointList.contains({1, 2}) ? "true" : "false") << std::endl;
	pointList.remove({1, 2});
	std::cout << "After remove({1,2}): ";
	pointList.print();

	//Test the iterator
	LinkedList<int> iterList;
	iterList.pushBack(10);
	iterList.pushBack(20);
	iterList.pushBack(30);
	std::cout << "Iterating: ";
	for (int val : iterList)  {
		std::cout << val << " ";
	}
	std::cout << std::endl;

	//Test the copy constructor (deep copy check)
	LinkedList<int> original;
	original.pushBack(1);
	original.pushBack(2);
	original.pushBack(3);
	LinkedList<int> copyList = original;
	copyList.pushBack(999);
	std::cout << "Original after copy modified: ";
	original.print();
	std::cout << "Copy: ";
	copyList.print();

	//Test the copy assignment operator
	LinkedList<int> assigned;
	assigned.pushBack(100);
	assigned = original;    // assigned should now match original (1 2 3)
	assigned .pushBack(4);
	std::cout << "Original after assignment target modified: ";
	original.print();
	std::cout << "Assigned: ";
	assigned.print();

	return 0;
}
