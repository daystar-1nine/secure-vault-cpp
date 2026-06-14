#include "../../include/vault/VaultManager.h"
#include <algorithm>   // for std::find_if

// 🔹 Add credential (prevent duplicates)
bool VaultManager::addCredential(const Credential& credential)
{
    if (exists(credential.getWebsite(), credential.getUsername()))
    {
        return false; // duplicate found
    }

    credentials.push_back(credential);
    return true;
}

// 🔹 Add or update credential (archives old password into history if updated)
bool VaultManager::addOrUpdateCredential(const Credential& credential)
{
    auto it = std::find_if(credentials.begin(), credentials.end(),
        [&](const Credential& c)
        {
            return c.getWebsite() == credential.getWebsite() && c.getUsername() == credential.getUsername();
        });

    if (it != credentials.end())
    {
        if (it->getPassword() != credential.getPassword())
        {
            std::string oldPass = it->getPassword();
            it->setPassword(credential.getPassword());
            it->addPasswordToHistory(oldPass);
        }
        for (const auto& p : credential.getPasswordHistory())
        {
            it->addPasswordToHistory(p);
        }
        return true;
    }

    credentials.push_back(credential);
    return true;
}

// 🔹 Remove credential by website + username
bool VaultManager::removeCredential(const std::string& website,
                                    const std::string& username)
{
    auto it = std::find_if(credentials.begin(), credentials.end(),
        [&](const Credential& c)
        {
            return c.getWebsite() == website && c.getUsername() == username;
        });

    if (it != credentials.end())
    {
        credentials.erase(it);
        return true;
    }

    return false;
}

// 🔹 Find credential (safe return)
std::optional<Credential> VaultManager::findCredential(const std::string& website,
                                                       const std::string& username) const
{
    auto it = std::find_if(credentials.begin(), credentials.end(),
        [&](const Credential& c)
        {
            return c.getWebsite() == website && c.getUsername() == username;
        });

    if (it != credentials.end())
    {
        return *it;
    }

    return std::nullopt;
}

// 🔹 Check existence
bool VaultManager::exists(const std::string& website,
                          const std::string& username) const
{
    return std::any_of(credentials.begin(), credentials.end(),
        [&](const Credential& c)
        {
            return c.getWebsite() == website && c.getUsername() == username;
        });
}

// 🔹 Get all (read-only)
const std::vector<Credential>& VaultManager::getAllCredentials() const
{
    return credentials;
}

// 🔹 Get size
size_t VaultManager::size() const
{
    return credentials.size();
}

// 🔹 Clear vault securely
void VaultManager::clearVault()
{
    for (auto& c : credentials)
    {
        c.clearSensitiveData(); // wipe passwords
    }

    credentials.clear();
}

// 🔹 Search by keyword
std::vector<Credential> VaultManager::search(const std::string& keyword) const
{
    std::vector<Credential> results;

    for (const auto& c : credentials)
    {
        if (c.getWebsite().find(keyword) != std::string::npos ||
            c.getUsername().find(keyword) != std::string::npos)
        {
            results.push_back(c);
        }
    }

    return results;
}