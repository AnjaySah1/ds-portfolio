#include "BST.h"
#include "Contact.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

// Contact directory driver. Reads one command per line from standard input.
// ADD  <first> <last> <phone>
// LIST
// COUNT
// QUIT

// Formats a contact as: Last, First phone

std::string formatContact(const Contact& c)
{
    return c.lastName + ", " + c.firstName + " " + c.phone;
}

// Turns a command word into upper case so "add" and "ADD" both work.
std::string toUpper(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    return text;
}

// Prints every contact in the vector, one per line.
void printContacts(const std::vector<Contact>& contacts)
{
    for (const Contact& c : contacts) {
        std::cout << "  " << formatContact(c) << std::endl;
    }
}

void handleAdd(BST<Contact>& directory, std::istringstream& args)
{
    std::string first, last, phone, extra;
    if (!(args >> first >> last >> phone) || (args >> extra)) {
        std::cout << "Usage: ADD <first> <last> <phone>" << std::endl;
        return;
    }

    Contact contact(first, last, phone);
    if (directory.search(contact)) {
        std::cout << "Already in the directory: " << last << ", " << first << " (not added)" << std::endl;
        return;
    }
    directory.insert(contact);
    std::cout << "Added: " << formatContact(contact) << std::endl;
}

void handleList(const BST<Contact>& directory)
{
    std::vector<Contact> all = directory.inorder();
    if (all.empty()) {
        std::cout << "The directory is empty." << std::endl;
        return;
    }
    printContacts(all);
}

void handleCount(const BST<Contact>& directory)
{
    std::cout << "Contacts: " << directory.size() << ", tree height: " << directory.height() << std::endl;
}

// Gets every contact with this exact last name. The tree is sorted by last name first, so all of them are next to each other.

std::vector<Contact> findByLastName(const BST<Contact>& directory, const std::string& lastName)
{
    return directory.collect([&lastName](const Contact& c) {
        return c.lastName.compare(lastName);
    });
}

// Looks up a last name and prints every contact that has it.
void handleLookup(const BST<Contact>& directory, std::istringstream& args)
{
    std::string last, extra;
    if (!(args >> last) || (args >> extra)) {
        std::cout << "Usage: LOOKUP <last>" << std::endl;
        return;
    }

    std::vector<Contact> matches = findByLastName(directory, last);
    if (matches.empty()) {
        std::cout << "Not found: no contact with last name " << last << std::endl;
        return;
    }
    std::cout << "Found " << matches.size() << " contact(s) with last name " << last << ":" << std::endl;
    printContacts(matches);
}


int main()
{
    BST<Contact> directory;

    // Checking if the user is entering commands directly.
    bool interactive = isatty(STDIN_FILENO);
    std::string line;

    if (interactive) {
        std::cout << "Contact directory. Type QUIT to exit." << std::endl;
    }

    while (true) {
        if (interactive) {
            std::cout << "> " << std::flush;
        }
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::istringstream args(line);
        std::string command;
        if (!(args >> command) || command[0] == '#') {
            continue;    // Blank lines and comment lines are skipped.
        }
        command = toUpper(command);

        // Print each command when reading input from a file.
        if (!interactive) {
            std::cout << "> " << line << std::endl;
        }

        if (command == "ADD") {
            handleAdd(directory, args);
        } else if (command == "LOOKUP") {
            handleLookup(directory, args);
        } else if (command == "LIST") {
            handleList(directory);
        } else if (command == "COUNT") {
            handleCount(directory);
        } else if (command == "QUIT" || command == "EXIT") {
            break;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
    return 0;
}
