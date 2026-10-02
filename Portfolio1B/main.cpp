#include "TextBuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//Parses one line of input and runs the matching TextBuffer command. Shared by both interactive mode(reading from the keyboard) and file mode(Reading from a command file), so the command logic only has to be written once.

void processLine(TextBuffer& buffer, const std::string& line, bool& shouldQuit)  {
     if (line.empty()) {
	return;
     }

     std::istringstream iss(line);
     std::string command;
     iss >> command;  // grabs the first word

     if (command == "TYPE")  {
	// Everything after "TYPE" is the text to add and grad the rest of the line.
	std::string rest;
	std::getline(iss, rest);
	if (!rest.empty() && rest[0] == ' ') {
	    rest = rest.substr(1);
	}
	buffer.typeText(rest);
     } else if (command == "DELETE") {
	int n;
	iss >> n;
	buffer.deleteChars(n);
     } else if (command == "UNDO") {
	buffer.undo();
     } else if (command == "REDO") {
	buffer.redo();
     } else if (command == "PRINT") {
	buffer.print();
     } else if (command == "QUIT") {
	shouldQuit = true;
     } else {
	std::cout << "Unknown command: " << command << std::endl;
     }
}

int main(int argc, char* argv[]) {
     TextBuffer buffer;
     bool shouldQuit = false;

     if (argc > 1) {
	// File mode: read commands from the file given as argv[1]
	std::ifstream inputFile(argv[1]);
	if (!inputFile) {
	    std::cout << "Could not open file: " << argv[1] << std::endl;
	    return 1;
	}

	std::string line;
	while (std::getline(inputFile, line) && !shouldQuit) {
	    processLine(buffer, line, shouldQuit);
	}

	//Report how many undo/redo operations ran and what the documend looked like at the end
	std::cout << "\n ---Summary------" << std::endl;
	std::cout << "Undo operations performed: " << buffer.getUndoCount() << std::endl;
	std::cout << "Redo operations performed: " << buffer.getRedoCount() << std::endl;
	std::cout << " Final document: \"" << buffer.getFinalDocument() << "\"" << std::endl;

      } else {
	// Interactive mode: read commands from standard  input
	std::cout << "Text Buffer - commands: TYPE <text>, DELETE <n>, UNDO, REDO, PRINT, QUIT" << std::endl;
	std::string line;
	while (std::getline(std::cin, line) && !shouldQuit) {
	      processLine(buffer, line, shouldQuit);
	}
     }

     return 0;
}
