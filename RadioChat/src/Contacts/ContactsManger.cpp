#include "ContactsManger.h"
#include "../Logger/Logger.h"

ContactsManager::ContactsManager()
{
}

ContactsManager::~ContactsManager()
{
}

void ContactsManager::init(const ContactsSettings &settings)
{
    LOG_INF("-- Initialize contacts --"); 
    settings_ = settings;
}

void ContactsManager::addContact(uint16_t address, const std::string &name)
{
}

const std::vector<Contact> &ContactsManager::getContacts() const
{
    return contacts_;
}
