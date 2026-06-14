#include "auth/AuthManager.h"
#include "monocypher.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

// 🚨 Secure file wiper
static void secureWipeFile(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path)) return;
    try
    {
        uintmax_t size = std::filesystem::file_size(path);
        std::ofstream file(path, std::ios::binary | std::ios::in | std::ios::out);
        if (file)
        {
            std::vector<char> buffer(65536, 0);
            uintmax_t bytesWritten = 0;
            while (bytesWritten < size)
            {
                uintmax_t chunk = std::min(size - bytesWritten, static_cast<uintmax_t>(buffer.size()));
                file.write(buffer.data(), chunk);
                bytesWritten += chunk;
            }
            file.flush();
            file.close();
        }
        std::filesystem::remove(path);
    }
    catch (...)
    {
        std::filesystem::remove(path);
    }
}

// 🔹 Check if config file exists
bool AuthManager::vaultExists() const
{
    return std::filesystem::exists("data/config.dat");
}

// 🔹 Create vault password (first run)
bool AuthManager::createVaultPassword(const std::string& password)
{
    if (vaultExists())
        return false;

    // 1. Generate a 16-byte cryptographically secure random salt
    std::random_device rd;
    std::vector<uint8_t> salt(16);
    for (size_t i = 0; i < salt.size(); ++i)
    {
        salt[i] = static_cast<uint8_t>(rd() & 0xFF);
    }
    std::string saltHex = toHex(salt);

    // 2. Hash password + salt using Argon2id via Monocypher v4 structs (V2: 16 MB blocks, 4 passes)
    std::vector<uint8_t> hash(32);
    std::vector<uint8_t> workArea(16 * 1024 * 1024); // 16 MB work area
    
    crypto_argon2_config config;
    config.algorithm = CRYPTO_ARGON2_ID;
    config.nb_blocks = 16384; // 16 MB
    config.nb_passes = 4;
    config.nb_lanes = 1;

    crypto_argon2_inputs inputs;
    inputs.pass = reinterpret_cast<const uint8_t*>(password.data());
    inputs.pass_size = static_cast<uint32_t>(password.size());
    inputs.salt = salt.data();
    inputs.salt_size = static_cast<uint32_t>(salt.size());

    crypto_argon2(hash.data(), hash.size(),
                  workArea.data(),
                  config,
                  inputs,
                  crypto_argon2_no_extras);

    std::string hashHex = toHex(hash);

    // 3. Save config with 0 failed attempts and 0 lockout time, and the version tag
    return saveConfig(saltHex, hashHex, 0, 0, "SV02");
}

// 🔹 Verify vault password
bool AuthManager::verifyVaultPassword(const std::string& password)
{
    if (isLocked())
        return false;

    std::ifstream file("data/config.dat");
    if (!file)
        return false;

    std::string line;
    if (!std::getline(file, line))
    {
        return false;
    }
    file.close(); // Close file before potential re-writing

    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, '|'))
    {
        parts.push_back(part);
    }

    if (parts.size() < 4)
        return false;

    std::string saltHex = parts[0];
    std::string storedHashHex = parts[1];
    std::string failedAttemptsStr = parts[2];
    std::string lockUntilStr = parts[3];
    std::string version = (parts.size() >= 5) ? parts[4] : "legacy";

    int currentFailedAttempts = 0;
    try {
        currentFailedAttempts = std::stoi(failedAttemptsStr);
    } catch (...) {}

    // Verify input password using Argon2id parameters based on version
    uint32_t blocks = 1024; // Default to V1 parameters (1 MB)
    uint32_t passes = 3;
    size_t workAreaSize = 1024 * 1024;

    if (version == "SV02")
    {
        blocks = 16384; // V2 parameters (16 MB)
        passes = 4;
        workAreaSize = 16 * 1024 * 1024;
    }

    std::vector<uint8_t> saltBytes = fromHex(saltHex);
    std::vector<uint8_t> hashBytes(32);
    std::vector<uint8_t> workArea(workAreaSize);
    
    crypto_argon2_config config;
    config.algorithm = CRYPTO_ARGON2_ID;
    config.nb_blocks = blocks;
    config.nb_passes = passes;
    config.nb_lanes = 1;

    crypto_argon2_inputs inputs;
    inputs.pass = reinterpret_cast<const uint8_t*>(password.data());
    inputs.pass_size = static_cast<uint32_t>(password.size());
    inputs.salt = saltBytes.data();
    inputs.salt_size = static_cast<uint32_t>(saltBytes.size());

    crypto_argon2(hashBytes.data(), hashBytes.size(),
                  workArea.data(),
                  config,
                  inputs,
                  crypto_argon2_no_extras);

    std::string inputHashHex = toHex(hashBytes);

    if (inputHashHex == storedHashHex)
    {
        // Reset failed attempts & lockout, preserving version
        saveConfig(saltHex, storedHashHex, 0, 0, version);
        return true;
    }

    // Increment failed attempts and calculate persistent lockout timestamp
    currentFailedAttempts++;
    long long lockUntilTimestamp = 0;
    
    if (currentFailedAttempts >= 10)
    {
        // 🚨 TRIGGER PANIC SELF-DESTRUCT 🚨
        secureWipeFile("data/config.dat");
        secureWipeFile("data/vault.dat");
        if (std::filesystem::exists("data/backups"))
        {
            try {
                for (const auto& entry : std::filesystem::directory_iterator("data/backups"))
                {
                    if (entry.is_regular_file())
                    {
                        secureWipeFile(entry.path());
                    }
                }
            } catch (...) {}
        }
        return false;
    }

    if (currentFailedAttempts >= 3)
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        int lockSeconds = 10;
        if (currentFailedAttempts >= 6) lockSeconds = 30;
        lockUntilTimestamp = now + lockSeconds;
    }

    saveConfig(saltHex, storedHashHex, currentFailedAttempts, lockUntilTimestamp, version);
    return false;
}

// 🔹 Check if user is temporarily locked
bool AuthManager::isLocked() const
{
    return getLockoutRemainingTime() > 0;
}

// 🔹 Get remaining lockout time in seconds
long long AuthManager::getLockoutRemainingTime() const
{
    std::ifstream file("data/config.dat");
    if (!file)
        return 0;

    std::string saltHex, storedHashHex;
    std::string failedAttemptsStr, lockUntilStr;
    
    if (std::getline(file, saltHex, '|') &&
        std::getline(file, storedHashHex, '|') &&
        std::getline(file, failedAttemptsStr, '|') &&
        std::getline(file, lockUntilStr))
    {
        try {
            long long lockUntilTimestamp = std::stoll(lockUntilStr);
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            if (now < lockUntilTimestamp)
            {
                return lockUntilTimestamp - now;
            }
        } catch (...) {}
    }
    return 0;
}

// 🔐 Persistent config helper
bool AuthManager::saveConfig(const std::string& salt, const std::string& hash, int failedAttempts, long long lockUntil, const std::string& version) const
{
    std::filesystem::create_directories("data");
    std::ofstream file("data/config.dat");
    if (!file)
        return false;

    file << salt << "|" << hash << "|" << failedAttempts << "|" << lockUntil;
    if (!version.empty())
    {
        file << "|" << version;
    }
    return true;
}

// 🔄 Convert binary vector -> HEX string
std::string AuthManager::toHex(const std::vector<uint8_t>& data) const
{
    std::ostringstream oss;
    for (uint8_t c : data)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

// 🔄 Convert HEX string -> binary vector
std::vector<uint8_t> AuthManager::fromHex(const std::string& hex) const
{
    std::vector<uint8_t> output;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        output.push_back(byte);
    }
    return output;
}