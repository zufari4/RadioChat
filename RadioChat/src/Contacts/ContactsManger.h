#pragma once

#include "ContactsSettings.h"
#include <string>
#include <vector>
#include <stdint.h>

class Settings;

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
    void init(Settings& settings);
    void addContact(uint16_t address, const std::string& name);
    const std::vector<Contact>& getContacts() const;
    
private:
    void loadSettings(Settings& settings);

    ContactsSettings settings_;
    std::vector<Contact> contacts_;
};