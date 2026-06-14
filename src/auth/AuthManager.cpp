#include "../../include/auth/AuthManager.h"

#include <filesystem>
#include <fstream>
#include <functional>
#include <chrono>

// 🔹 Check if config file exists
bool AuthManager::vaultExists() const
{
    return std::filesystem::exists("data/config.dat");
}

// 🔹 Generate simple salt
std::string AuthManager::generateSalt() const
{
    return std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

// 🔹 Hash password with salt
std::string AuthManager::hashPassword(const std::string& password) const
{
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

// 🔹 Create vault password (first run)
bool AuthManager::createVaultPassword(const std::string& password)
{
    if (vaultExists())
        return false;

    std::ofstream file("data/config.dat");

    if (!file)
        return false;

    std::string salt = generateSalt();
    std::string hashed = hashPassword(password + salt);

    // Store: salt|hash
    file << salt << "|" << hashed;

    return true;
}

// 🔹 Verify vault password
bool AuthManager::verifyVaultPassword(const std::string& password)
{
    if (isLocked())
        return false;

    std::ifstream file("data/config.dat");

    if (!file)
        return false;

    std::string salt, storedHash;

    std::getline(file, salt, '|');
    std::getline(file, storedHash);

    std::string inputHash = hashPassword(password + salt);

    if (inputHash == storedHash)
    {
        resetAttempts();
        return true;
    }

    // ❌ Failed attempt
    failedAttempts++;

    // ⏳ Progressive lock
    if (failedAttempts >= 3)
    {
        lockUntil = std::chrono::system_clock::now() + std::chrono::seconds(10);
    }
    if (failedAttempts >= 6)
    {
        lockUntil = std::chrono::system_clock::now() + std::chrono::seconds(30);
    }
    if (failedAttempts >= 10)
    {
        lockUntil = std::chrono::system_clock::now() + std::chrono::minutes(1);
    }

    return false;
}

// 🔹 Reset attempts after success
void AuthManager::resetAttempts()
{
    failedAttempts = 0;
}

// 🔹 Check if locked
bool AuthManager::isLocked() const
{
    return std::chrono::system_clock::now() < lockUntil;
}