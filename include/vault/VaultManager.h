#ifndef VAULT_MANAGER_H
#define VAULT_MANAGER_H

#include <vector>
#include <string>
#include <optional>

#include "../models/Credential.h"

/*
    VaultManager handles all in-memory credential operations.

    Key Improvements:
    - Safe return types (no raw pointers)
    - Unique identification using website + username
    - Duplicate prevention
*/
class VaultManager
{
private:
    std::vector<Credential> credentials;

public:
    // 🔹 Add credential (returns false if duplicate)
    bool addCredential(const Credential& credential);

    // 🔹 Add or update credential (archives old password into history if updated)
    bool addOrUpdateCredential(const Credential& credential);

    // 🔹 Remove credential using website + username
    bool removeCredential(const std::string& website,
                          const std::string& username);

    // 🔹 Find credential safely
    std::optional<Credential> findCredential(const std::string& website,
                                             const std::string& username) const;

    // 🔹 Check if credential exists
    bool exists(const std::string& website,
                const std::string& username) const;

    // 🔹 Get all credentials (read-only)
    const std::vector<Credential>& getAllCredentials() const;

    // 🔹 Get total count
    size_t size() const;

    // 🔹 Clear vault securely
    void clearVault();

    // 🔹 Search by keyword
    std::vector<Credential> search(const std::string& keyword) const;
};

#endif