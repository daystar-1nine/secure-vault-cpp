#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

/*
    Encryption module for vault data.

    Version 1:
    - XOR-based encryption (learning purpose)

    Design Goals:
    - Symmetric encryption (same key for encrypt/decrypt)
    - Safe storage (encoded output)
    - Future upgrade ready (AES, ChaCha20)
*/
class Encryption
{
public:
    // 🔹 Encrypt plain text using password-derived key
    std::string encrypt(
        const std::string& plainText,
        const std::string& password
    ) const;

    // 🔹 Decrypt cipher text
    std::string decrypt(
        const std::string& cipherText,
        const std::string& password
    ) const;

private:
    // 🔐 Core XOR logic
    std::string xorCipher(
        const std::string& data,
        const std::string& key
    ) const;

    // 🔑 Derive key from password
    std::string deriveKey(const std::string& password) const;

    // 🔄 Encoding helpers (for safe storage)
    std::string toHex(const std::string& input) const;
    std::string fromHex(const std::string& input) const;
};

#endif