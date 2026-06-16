#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>

/*
    AuthManager handles:
    - Vault password setup & verification using Argon2id
    - Persistent failed attempt tracking and lockouts in config.dat
*/
class AuthManager
{
public:
    // 🔹 Check if vault is already initialized
    bool vaultExists() const;

    // 🔹 Create vault password (first run), returns generated recovery key
    std::string createVaultPassword(const std::string& password);

    // 🔹 Verify vault password
    bool verifyVaultPassword(const std::string& password);

    // 🔹 Change master password (requires current password verification)
    bool changeMasterPassword(const std::string& currentPassword, const std::string& newPassword);

    // 🔹 Reset master password via recovery key
    bool resetMasterPasswordWithKey(const std::string& recoveryKey, const std::string& newPassword);

    // 🔹 Get recovery key (for display purposes)
    std::string getRecoveryKey() const;

    // 🔹 Check if user is temporarily locked
    bool isLocked() const;

    // 🔹 Get remaining lockout time in seconds
    long long getLockoutRemainingTime() const;

private:
    // 🔐 Persistent config helper
    bool saveConfig(const std::string& salt, const std::string& hash, int failedAttempts, long long lockUntil, const std::string& version = "", const std::string& recoveryKey = "") const;

    // 🔄 Hex helpers
    std::string toHex(const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> fromHex(const std::string& hex) const;
};

#endif