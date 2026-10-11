#ifndef CONTACT_H
#define CONTACT_H

#include <string>
// One entry in the directory. Contacts are ordered by last name, and by first name when the last names are equal.
// The phone number is not part of the ordering, so two contacts with the same first and last name count as the same contact.

struct Contact {
    std::string firstName;
    std::string lastName;
    std::string phone;

    Contact() {}

    Contact(const std::string& first, const std::string& last, const std::string& ph)
    : firstName(first), lastName(last), phone(ph) {}

    // The only comparison BST<Contact> needs.
    bool operator<(const Contact& other) const
    {
    if (lastName != other.lastName) {
        return lastName < other.lastName;
    }
    return firstName < other.firstName;
    }
};

#endif  // End of  header guard
