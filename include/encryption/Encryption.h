#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

/*
    Encryption module for vault data.
    - Uses ChaCha20-Poly1305 authenticated symmetric encryption (AEAD)
    - Uses Argon2id key derivation to derive the 256-bit encryption key
    - Appends a random salt and nonce to the ciphertext for safe storage
*/
class Encryption
{
public:
    // 🔹 Encrypt plain text using password-derived key
    std::string encrypt(
        const std::string& plainText,
        const std::string& password
    ) const;

    // 🔹 Decrypt cipher text (returns std::nullopt if decryption/authentication fails)
    std::optional<std::string> decrypt(
        const std::string& cipherText,
        const std::string& password
    ) const;

private:
    // 🔄 Hex helpers (renamed to take std::vector for modern usage)
    std::string toHex(const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> fromHex(const std::string& hex) const;
};

#endif