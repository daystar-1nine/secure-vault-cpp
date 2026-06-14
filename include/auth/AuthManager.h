#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <string>
#include <chrono>

/*
    AuthManager handles:
    - Vault password setup & verification
    - Failed attempt tracking
    - Lockout mechanism
    - Recovery support (future extension)
*/
class AuthManager
{
public:
    // 🔹 Check if vault is already initialized
    bool vaultExists() const;

    // 🔹 Create vault password (first run)
    bool createVaultPassword(const std::string& password);

    // 🔹 Verify vault password
    bool verifyVaultPassword(const std::string& password);

    // 🔹 Reset attempt counter (on successful login)
    void resetAttempts();

    // 🔹 Check if user is temporarily locked
    bool isLocked() const;

private:
    // 🔐 Security helpers
    std::string hashPassword(const std::string& password) const;
    std::string generateSalt() const;

    // 🔒 Attempt tracking
    int failedAttempts = 0;

    // ⏳ Lock mechanism
    std::chrono::system_clock::time_point lockUntil;
};

#endif