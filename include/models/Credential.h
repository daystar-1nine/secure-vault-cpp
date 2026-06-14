#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <string>
#include <sstream>

/*
    Credential class represents a single entry
    in the password vault.

    Design goals:
    - Encapsulation (data hidden)
    - Easy serialization (for file storage)
    - Security awareness (clear sensitive data)
*/
class Credential
{
private:
    std::string website;
    std::string username;
    std::string password;

public:
    // 🔹 Constructors
    Credential() = default;

    Credential(const std::string& site,
               const std::string& user,
               const std::string& pass)
        : website(site), username(user), password(pass) {}

    // 🔹 Getters (read-only access)
    std::string getWebsite() const { return website; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }

    // 🔹 Setters
    void setWebsite(const std::string& site) { website = site; }
    void setUsername(const std::string& user) { username = user; }
    void setPassword(const std::string& pass) { password = pass; }

    // 🔹 Convert to string for file storage
    std::string serialize() const
    {
        return website + "|" + username + "|" + password;
    }

    // 🔹 Load from string (reverse of serialize)
    static Credential deserialize(const std::string& data)
    {
        std::stringstream ss(data);
        std::string site, user, pass;

        std::getline(ss, site, '|');
        std::getline(ss, user, '|');
        std::getline(ss, pass, '|');

        return Credential(site, user, pass);
    }

    // 🔹 Clear sensitive data from memory
    void clearSensitiveData()
    {
        if (!password.empty())
        {
            volatile char* p = const_cast<volatile char*>(password.data());
            size_t size = password.size();
            while (size--)
            {
                *p++ = 0;
            }
            password.clear();
        }
    }
};

#endif