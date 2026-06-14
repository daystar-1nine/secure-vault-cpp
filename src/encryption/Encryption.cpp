#include "encryption/Encryption.h"
#include "monocypher.h"

#include <random>
#include <sstream>
#include <iomanip>

// 🔹 Encrypt plain text using ChaCha20-Poly1305 (direct lock)
std::string Encryption::encrypt(
    const std::string& plainText,
    const std::string& password
) const
{
    // 1. Generate random 16-byte salt and 24-byte nonce
    std::random_device rd;
    std::vector<uint8_t> salt(16);
    std::vector<uint8_t> nonce(24);
    for (size_t i = 0; i < salt.size(); ++i) salt[i] = static_cast<uint8_t>(rd() & 0xFF);
    for (size_t i = 0; i < nonce.size(); ++i) nonce[i] = static_cast<uint8_t>(rd() & 0xFF);

    // 2. Derive 32-byte key from password using Argon2id via Monocypher v4 structs (V2: 16 MB blocks, 4 passes)
    std::vector<uint8_t> key(32);
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

    crypto_argon2(key.data(), key.size(),
                  workArea.data(),
                  config,
                  inputs,
                  crypto_argon2_no_extras);

    // 3. Encrypt using ChaCha20-Poly1305 AEAD lock
    std::vector<uint8_t> ciphertext(plainText.size());
    std::vector<uint8_t> mac(16);
    
    crypto_aead_lock(ciphertext.data(),
                     mac.data(),
                     key.data(),
                     nonce.data(),
                     NULL, 0, // No associated data
                     reinterpret_cast<const uint8_t*>(plainText.data()), plainText.size());

    // 4. Combine: salt (16 bytes) + nonce (24 bytes) + MAC (16 bytes) + ciphertext
    std::vector<uint8_t> outBytes;
    outBytes.reserve(salt.size() + nonce.size() + mac.size() + ciphertext.size());
    outBytes.insert(outBytes.end(), salt.begin(), salt.end());
    outBytes.insert(outBytes.end(), nonce.begin(), nonce.end());
    outBytes.insert(outBytes.end(), mac.begin(), mac.end());
    outBytes.insert(outBytes.end(), ciphertext.begin(), ciphertext.end());

    // 5. Convert combined bytes to hex string and prepend the V2 version tag
    return "SV02|" + toHex(outBytes);
}

// 🔹 Decrypt cipher text (checks MAC integrity, returns std::nullopt on failure)
std::optional<std::string> Encryption::decrypt(
    const std::string& cipherText,
    const std::string& password
) const
{
    std::string payloadHex = cipherText;
    uint32_t blocks = 1024; // Default to V1 parameters (1 MB)
    uint32_t passes = 3;
    size_t workAreaSize = 1024 * 1024;

    // Detect version tag
    if (cipherText.rfind("SV02|", 0) == 0)
    {
        payloadHex = cipherText.substr(5);
        blocks = 16384; // V2 parameters (16 MB)
        passes = 4;
        workAreaSize = 16 * 1024 * 1024;
    }

    // 1. Convert hex string to bytes
    std::vector<uint8_t> cipherBytes = fromHex(payloadHex);
    
    // Check minimum size (salt=16 + nonce=24 + mac=16)
    if (cipherBytes.size() < 16 + 24 + 16)
    {
        return std::nullopt;
    }

    // 2. Extract salt, nonce, MAC, and ciphertext
    std::vector<uint8_t> salt(cipherBytes.begin(), cipherBytes.begin() + 16);
    std::vector<uint8_t> nonce(cipherBytes.begin() + 16, cipherBytes.begin() + 16 + 24);
    std::vector<uint8_t> mac(cipherBytes.begin() + 16 + 24, cipherBytes.begin() + 16 + 24 + 16);
    std::vector<uint8_t> ciphertext(cipherBytes.begin() + 16 + 24 + 16, cipherBytes.end());

    // 3. Derive 32-byte key from password and extracted salt using Argon2id via Monocypher v4 structs
    std::vector<uint8_t> key(32);
    std::vector<uint8_t> workArea(workAreaSize);
    
    crypto_argon2_config config;
    config.algorithm = CRYPTO_ARGON2_ID;
    config.nb_blocks = blocks;
    config.nb_passes = passes;
    config.nb_lanes = 1;

    crypto_argon2_inputs inputs;
    inputs.pass = reinterpret_cast<const uint8_t*>(password.data());
    inputs.pass_size = static_cast<uint32_t>(password.size());
    inputs.salt = salt.data();
    inputs.salt_size = static_cast<uint32_t>(salt.size());

    crypto_argon2(key.data(), key.size(),
                  workArea.data(),
                  config,
                  inputs,
                  crypto_argon2_no_extras);

    // 4. Decrypt and verify integrity using ChaCha20-Poly1305 AEAD unlock
    std::vector<uint8_t> plaintext(ciphertext.size());
    
    int decryptResult = crypto_aead_unlock(plaintext.data(),
                                           mac.data(),
                                           key.data(),
                                           nonce.data(),
                                           NULL, 0,
                                           ciphertext.data(), ciphertext.size());

    if (decryptResult != 0)
    {
        // Authentication or decryption failed (invalid password or corrupted file)
        return std::nullopt;
    }

    return std::string(plaintext.begin(), plaintext.end());
}

// 🔄 Convert binary vector -> HEX string
std::string Encryption::toHex(const std::vector<uint8_t>& data) const
{
    std::ostringstream oss;
    for (uint8_t c : data)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

// 🔄 Convert HEX string -> binary vector
std::vector<uint8_t> Encryption::fromHex(const std::string& hex) const
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