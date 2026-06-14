#include "../../include/storage/StorageManager.h"

#include <fstream>
#include <sstream>
#include <filesystem>

// 🔹 Save vault with atomic write
bool StorageManager::saveVault(
    const std::vector<Credential>& credentials,
    const std::string& password
)
{
    // Serialize all credentials
    std::string data = serializeAll(credentials);

    // Encrypt data
    std::string encryptedData = encryption.encrypt(data, password);

    // Write to temp file first
    std::ofstream tempFile(TEMP_FILE, std::ios::binary);
    if (!tempFile)
        return false;

    tempFile << encryptedData;
    tempFile.close();

    // Replace original file (atomic)
    std::filesystem::rename(TEMP_FILE, VAULT_FILE);

    return true;
}

// 🔹 Load vault safely
std::optional<std::vector<Credential>> StorageManager::loadVault(
    const std::string& password
)
{
    std::ifstream file(VAULT_FILE, std::ios::binary);

    if (!file)
        return std::nullopt;

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Decrypt data
    std::string decrypted = encryption.decrypt(buffer.str(), password);

    // Deserialize
    auto credentials = deserializeAll(decrypted);

    return credentials;
}

// 🔒 Serialize all credentials
std::string StorageManager::serializeAll(
    const std::vector<Credential>& credentials
) const
{
    std::ostringstream out;

    for (const auto& c : credentials)
    {
        out << c.serialize() << '\n';
    }

    return out.str();
}

// 🔄 Deserialize all credentials
std::vector<Credential> StorageManager::deserializeAll(
    const std::string& data
) const
{
    std::vector<Credential> credentials;

    std::stringstream ss(data);
    std::string line;

    while (std::getline(ss, line))
    {
        if (line.empty())
            continue;

        credentials.push_back(
            Credential::deserialize(line)
        );
    }

    return credentials;
}