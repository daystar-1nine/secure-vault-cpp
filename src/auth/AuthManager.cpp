#include "auth/AuthManager.h"
#include "monocypher.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>

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

    // 2. Hash password + salt using Argon2id via Monocypher v4 structs
    std::vector<uint8_t> hash(32);
    std::vector<uint8_t> workArea(1024 * 1024); // 1 MB work area
    
    crypto_argon2_config config;
    config.algorithm = CRYPTO_ARGON2_ID;
    config.nb_blocks = 1024; // 1 MB
    config.nb_passes = 3;
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

    // 3. Save config with 0 failed attempts and 0 lockout time
    return saveConfig(saltHex, hashHex, 0, 0);
}

// 🔹 Verify vault password
bool AuthManager::verifyVaultPassword(const std::string& password)
{
    if (isLocked())
        return false;

    std::ifstream file("data/config.dat");
    if (!file)
        return false;

    std::string saltHex, storedHashHex;
    std::string failedAttemptsStr, lockUntilStr;
    
    if (!std::getline(file, saltHex, '|') ||
        !std::getline(file, storedHashHex, '|') ||
        !std::getline(file, failedAttemptsStr, '|') ||
        !std::getline(file, lockUntilStr))
    {
        return false;
    }
    file.close(); // Close file before potential re-writing

    int currentFailedAttempts = 0;
    try {
        currentFailedAttempts = std::stoi(failedAttemptsStr);
    } catch (...) {}

    // Verify input password using Argon2id via Monocypher v4 structs
    std::vector<uint8_t> saltBytes = fromHex(saltHex);
    std::vector<uint8_t> hashBytes(32);
    std::vector<uint8_t> workArea(1024 * 1024); // 1 MB work area
    
    crypto_argon2_config config;
    config.algorithm = CRYPTO_ARGON2_ID;
    config.nb_blocks = 1024; // 1 MB
    config.nb_passes = 3;
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
        // Reset failed attempts & lockout
        saveConfig(saltHex, storedHashHex, 0, 0);
        return true;
    }

    // Increment failed attempts and calculate persistent lockout timestamp
    currentFailedAttempts++;
    long long lockUntilTimestamp = 0;
    
    if (currentFailedAttempts >= 3)
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        int lockSeconds = 10;
        if (currentFailedAttempts >= 6) lockSeconds = 30;
        if (currentFailedAttempts >= 10) lockSeconds = 60;
        lockUntilTimestamp = now + lockSeconds;
    }

    saveConfig(saltHex, storedHashHex, currentFailedAttempts, lockUntilTimestamp);
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
bool AuthManager::saveConfig(const std::string& salt, const std::string& hash, int failedAttempts, long long lockUntil) const
{
    std::filesystem::create_directories("data");
    std::ofstream file("data/config.dat");
    if (!file)
        return false;

    file << salt << "|" << hash << "|" << failedAttempts << "|" << lockUntil;
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