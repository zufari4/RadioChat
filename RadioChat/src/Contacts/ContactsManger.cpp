#include "ContactsManger.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"

ContactsManager::ContactsManager()
{
}

ContactsManager::~ContactsManager()
{
}

void ContactsManager::init(Settings &settings)
{
    LOG_INF("-- Initialize contacts --"); 
    loadSettings(settings);
}

void ContactsManager::addContact(uint16_t address, const std::string &name)
{
}

const std::vector<Contact> &ContactsManager::getContacts() const
{
    return contacts_;
}

void ContactsManager::loadSettings(Settings& settings)
{
    auto props = settings.contacts();
    settings_.filename = Settings::get_s(eContactsFilename, props);
    settings_.path = Settings::get_s(eContactsPath, props);
}
