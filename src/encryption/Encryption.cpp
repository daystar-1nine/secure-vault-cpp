#include "../../include/encryption/Encryption.h"

#include <sstream>
#include <iomanip>
#include <functional>

// 🔹 Encrypt: plain → key → XOR → HEX
std::string Encryption::encrypt(
    const std::string& plainText,
    const std::string& password
) const
{
    std::string key = deriveKey(password);
    std::string encrypted = xorCipher(plainText, key);
    return toHex(encrypted);
}

// 🔹 Decrypt: HEX → XOR → plain
std::string Encryption::decrypt(
    const std::string& cipherText,
    const std::string& password
) const
{
    std::string key = deriveKey(password);
    std::string decoded = fromHex(cipherText);
    return xorCipher(decoded, key);
}

// 🔐 Core XOR logic
std::string Encryption::xorCipher(
    const std::string& data,
    const std::string& key
) const
{
    std::string result = data;

    for (size_t i = 0; i < data.size(); i++)
    {
        result[i] = data[i] ^ key[i % key.size()];
    }

    return result;
}

// 🔑 Derive key from password
std::string Encryption::deriveKey(const std::string& password) const
{
    std::hash<std::string> hasher;
    size_t hash = hasher(password);

    return std::to_string(hash);
}

// 🔄 Convert binary → HEX (safe for file)
std::string Encryption::toHex(const std::string& input) const
{
    std::ostringstream oss;

    for (unsigned char c : input)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }

    return oss.str();
}

// 🔄 Convert HEX → binary
std::string Encryption::fromHex(const std::string& input) const
{
    std::string output;

    for (size_t i = 0; i < input.length(); i += 2)
    {
        std::string byteString = input.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        output.push_back(byte);
    }

    return output;
}