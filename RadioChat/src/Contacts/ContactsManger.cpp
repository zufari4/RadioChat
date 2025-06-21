#include "ContactsManger.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"
#include "../Configuration.h"
#include "../Flash/Flash.h"
#include "../Utils.h"
#include <algorithm>

ContactsManager::ContactsManager() = default;
ContactsManager::~ContactsManager() = default;

void ContactsManager::init(Settings &settings)
{
    LOG_INF("-- Initialize contacts --"); 
    loadSettings(settings);
}

void ContactsManager::addContact(uint16_t address, const std::string &name)
{
    if (name.empty() && address == 0) {
        LOG_ERR("Can't add contact with empty name and address");
        return;
    }
    Contact contact{address, name};
    auto line = stringify(contact);
    LOG_DBG("Adding contact: %s", line.c_str());
    if (!FLASH.append(getFilename(), line + "\n")) {
        LOG_ERR("Can't add contact '%s' to file '%s'", line.c_str(), getFilename().c_str());
    }
    else {
        LOG_INF("Contact added successfully");
    }
}

void ContactsManager::removeContact(uint16_t address)
{
    LOG_INF("Removing contact with address %u", address);
    auto contacts = getContacts();
    auto it = std::remove_if(contacts.begin(), contacts.end(),
        [address](const Contact& c) { return c.address == address; });
    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        LOG_DBG("Contact with address %u removed", address);
    }
    else {
        LOG_ERR("Contact with address %u not found", address);
        return;
    }
    // Rewrite the file with updated contacts
    FLASH.remove(getFilename());
    for (const auto& contact : contacts) {
        FLASH.append(getFilename(), stringify(contact) + "\n");
    }
    LOG_INF("Contacts updated, %zu contacts remaining", contacts.size());
}

void ContactsManager::editContact(uint16_t address, const std::string& name)
{
    LOG_DBG("Editing contact with address %u to name '%s'", address, name.c_str());
    removeContact(address);
    addContact(address, name);
}

ContactList ContactsManager::getContacts() const
{
    ContactList contacts;
    auto lines = FLASH.readLastNLines(getFilename(), 0, 0xFFFFFFFF);
    for (const auto& line : lines) {
        if (!line.empty()) {
            auto contact = parseContact(line);
            if (contact.address != 0 || !contact.name.empty()) {
                contacts.push_back(contact);
            }
        }
    }
    if (contacts.empty()) {
        LOG_DBG("No contacts found");
    }
    else {
        LOG_DBG("Found %zu contacts", contacts.size());
    }
    return contacts;
}

Contact ContactsManager::getContact(uint16_t address) const
{
    auto contacts = getContacts();
    auto it = std::find_if(contacts.begin(), contacts.end(),
        [address](const Contact& c) { return c.address == address; });
    if (it != contacts.end()) {
        return *it;
    }
    LOG_DBG("Contact with address %u not found", address);
    return Contact();
}

void ContactsManager::loadSettings(Settings& settings)
{
    auto props = settings.contacts();
    settings_.filename = Settings::get_s(eContactsFilename, props);
    settings_.path = Settings::get_s(eContactsPath, props);
}

std::string ContactsManager::getFilename() const
{
    return STORAGE_DIR "/" + settings_.path + "/" + settings_.filename;
}

Contact ContactsManager::parseContact(const std::string& line) const
{
    Contact contact;
    auto parts = utils::split(line, '\t');
    if (parts.size() >= 2) {
        contact.address = static_cast<uint16_t>(std::stoi(parts[0]));
        contact.name = parts[1];
    }
    else {
        LOG_ERR("Can't parse contact from line '%s'", line.c_str());
    }
    return contact;
}

std::string ContactsManager::stringify(const Contact& contact) const
{
    return std::to_string(contact.address) + "\t" + contact.name;
}
