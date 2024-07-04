#pragma once

#include "ContactsSettings.h"
#include <string>
#include <vector>
#include <stdint.h>

struct Contact
{
    uint16_t address;
    std::string name;
};

class ContactsManager
{
public:
    ContactsManager();
    ~ContactsManager();
    void init(const ContactsSettings& settings);
    void addContact(uint16_t address, const std::string& name);
    const std::vector<Contact>& getContacts() const;
    
private:
    ContactsSettings settings_;
    std::vector<Contact> contacts_;
};