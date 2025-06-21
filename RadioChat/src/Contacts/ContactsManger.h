#pragma once

#include "ContactsSettings.h"
#include <string>
#include <vector>
#include <stdint.h>

class Settings;

struct Contact
{
    uint16_t address = 0;
    std::string name;
};

using ContactList = std::vector<Contact>;

class ContactsManager
{
public:
    ContactsManager();
    ~ContactsManager();
    void init(Settings& settings);
    void addContact(uint16_t address, const std::string& name);
    void removeContact(uint16_t address);
    void editContact(uint16_t address, const std::string& name);
    ContactList getContacts() const;
    Contact getContact(uint16_t address) const;
    std::string stringify(const Contact& contact) const;

private:
    void loadSettings(Settings& settings);
    std::string getFilename() const;
    Contact parseContact(const std::string& line) const;

    ContactsSettings settings_;
};