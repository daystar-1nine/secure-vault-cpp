#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <vector>
#include <optional>

#include "../models/Credential.h"
#include "../encryption/Encryption.h"

/*
    StorageManager handles:
    - Saving encrypted vault data
    - Loading and decrypting vault data
    - Safe file operations
*/
class StorageManager
{
public:
    // 🔹 Save credentials securely
    bool saveVault(
        const std::vector<Credential>& credentials,
        const std::string& password
    );

    // 🔹 Load credentials safely
    std::optional<std::vector<Credential>> loadVault(
        const std::string& password
    );

private:
    static constexpr const char* VAULT_FILE = "data/vault.dat";
    static constexpr const char* TEMP_FILE  = "data/vault.tmp";

    Encryption encryption;

    // 🔒 Helper: atomic file write
    bool safeWrite(const std::string& data);

    // 🔍 Helper: serialize all credentials
    std::string serializeAll(const std::vector<Credential>& credentials) const;

    // 🔄 Helper: deserialize
    std::vector<Credential> deserializeAll(const std::string& data) const;
};

#endif