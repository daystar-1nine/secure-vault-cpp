#include "../../include/storage/StorageManager.h" // rebuild trigger

#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <algorithm>

// 🔹 Save vault with atomic write, automatic backups, and optional recovery key
bool StorageManager::saveVault(
    const std::vector<Credential>& credentials,
    const std::string& password,
    const std::string& /*recoveryKey*/  // recovery key stored in config.dat by AuthManager
)
{
    std::filesystem::create_directories("data");

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

    // 📂 Back up the current vault file if it exists before overwriting
    if (std::filesystem::exists(VAULT_FILE))
    {
        try
        {
            std::filesystem::create_directories("data/backups");
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string backupPath = "data/backups/vault_" + std::to_string(now) + ".dat";
            std::filesystem::copy_file(VAULT_FILE, backupPath, std::filesystem::copy_options::overwrite_existing);

            // Rotate backups (keep only latest 5)
            std::vector<std::filesystem::path> backups;
            for (const auto& entry : std::filesystem::directory_iterator("data/backups"))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".dat")
                {
                    backups.push_back(entry.path());
                }
            }

            if (backups.size() > 5)
            {
                // Sort by last write time
                std::sort(backups.begin(), backups.end(), [](const auto& a, const auto& b) {
                    return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                });
                // Remove oldest backups
                for (size_t i = 0; i < backups.size() - 5; ++i)
                {
                    std::filesystem::remove(backups[i]);
                }
            }
        }
        catch (...) {}
    }

    // Replace original file (atomic)
    std::filesystem::rename(TEMP_FILE, VAULT_FILE);

    return true;
}

// 🔹 Compatibility overload (no recovery key needed - stored separately in config.dat)
bool StorageManager::saveVault(
    const std::vector<Credential>& credentials,
    const std::string& password
)
{
    return saveVault(credentials, password, "");
}

// 🔹 Load vault safely, returns credentials + stored recovery key (always "" here, key is in config.dat)
std::optional<std::pair<std::vector<Credential>, std::string>> StorageManager::loadVault(
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
    auto decrypted = encryption.decrypt(buffer.str(), password);
    if (!decrypted)
    {
        return std::nullopt; // Decryption or validation failed
    }

    // Deserialize
    auto credentials = deserializeAll(*decrypted);

    return std::make_pair(credentials, std::string(""));
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