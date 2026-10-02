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
	intList.print();

	//Test with string
	LinkedList<std::string> stringList;
	stringList.pushFront("World");
	stringList.pushFront("Hello");
	stringList.print();

	//Test with custom struct(Point): porves the template works for more than just built-in types, as required by the assignment
	LinkedList<Point> pointList;
	pointList.pushBack({1, 2});
	pointList.pushBack({3, 4});
	pointList.print();

	//Test the iterator by using the list in a range-based for loop
	std::cout << " Itereating int list: ";
	for (int val : intList) {
	    std::cout << val << " ";
	}
	std::cout << std::endl;

	//Test the copy constructor: modifying the copy should NOT affect the original. If it did, that would mean the copy is sharing memory with the original instead of having its own separate nodes.

	LinkedList<int> copyList = intList;
	copyList.pushBack(999);
	std::cout << "Original after copy modified: ";
	intList.print();
	std::cout << "copy: ";
	copyList.print();

	return 0;
}
