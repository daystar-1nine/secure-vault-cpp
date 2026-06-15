#include "utils/CryptoUtils.h"
#include <cstring>

// 🔑 Self-contained SHA-1 implementation
struct SHA1
{
    uint32_t state[5];
    uint32_t count[2];
    uint8_t buffer[64];

    void init()
    {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
        state[4] = 0xC3D2E1F0;
        count[0] = count[1] = 0;
    }

    static uint32_t rol(uint32_t value, uint32_t bits)
    {
        return (value << bits) | (value >> (32 - bits));
    }

    void transform(const uint8_t* block)
    {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
        {
            w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | block[i * 4 + 3];
        }
        for (int i = 16; i < 80; i++)
        {
            w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

        for (int i = 0; i < 20; i++)
        {
            uint32_t temp = rol(a, 5) + ((b & c) | (~b & d)) + e + w[i] + 0x5A827999;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 20; i < 40; i++)
        {
            uint32_t temp = rol(a, 5) + (b ^ c ^ d) + e + w[i] + 0x6ED9EBA1;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 40; i < 60; i++)
        {
            uint32_t temp = rol(a, 5) + ((b & c) | (b & d) | (c & d)) + e + w[i] + 0x8F1BBCDC;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 60; i < 80; i++)
        {
            uint32_t temp = rol(a, 5) + (b ^ c ^ d) + e + w[i] + 0xCA62C1D6;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
    }

    void update(const uint8_t* data, uint32_t len)
    {
        uint32_t j = (count[0] >> 3) & 63;
        if ((count[0] += len << 3) < (len << 3)) count[1]++;
        count[1] += (len >> 29);
        uint32_t i = 0;
        if ((j + len) > 63)
        {
            memcpy(&buffer[j], data, (i = 64 - j));
            transform(buffer);
            for (; i + 63 < len; i += 64)
            {
                transform(&data[i]);
            }
            j = 0;
        }
        memcpy(&buffer[j], &data[i], len - i);
    }

    void final(uint8_t digest[20])
    {
        uint8_t finalcount[8];
        for (int i = 0; i < 8; i++)
        {
            finalcount[i] = static_cast<uint8_t>((count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);
        }
        uint8_t c = 0200;
        update(&c, 1);
        while ((count[0] & 504) != 448)
        {
            uint8_t z = 0;
            update(&z, 1);
        }
        update(finalcount, 8);
        for (int i = 0; i < 20; i++)
        {
            digest[i] = static_cast<uint8_t>((state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
        }
    }
};

// 🔑 HMAC-SHA1 Implementation
static void hmacSha1(const uint8_t* key, size_t keyLen, const uint8_t* msg, size_t msgLen, uint8_t output[20])
{
    uint8_t k[64] = {0};
    if (keyLen > 64)
    {
        SHA1 sha;
        sha.init();
        sha.update(key, keyLen);
        sha.final(k);
    }
    else
    {
        memcpy(k, key, keyLen);
    }

    uint8_t ipad[64];
    uint8_t opad[64];
    for (int i = 0; i < 64; i++)
    {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5C;
    }

    SHA1 shaInner;
    shaInner.init();
    shaInner.update(ipad, 64);
    shaInner.update(msg, msgLen);
    uint8_t innerDigest[20];
    shaInner.final(innerDigest);

    SHA1 shaOuter;
    shaOuter.init();
    shaOuter.update(opad, 64);
    shaOuter.update(innerDigest, 20);
    shaOuter.final(output);
}

// 🔑 Base32 Decoder for TOTP Secrets
std::vector<uint8_t> base32Decode(const std::string& input)
{
    std::vector<uint8_t> result;
    int buffer = 0;
    int bitsLeft = 0;
    for (char c : input)
    {
        if (c == ' ' || c == '-') continue;
        int val = 0;
        if (c >= 'A' && c <= 'Z') val = c - 'A';
        else if (c >= 'a' && c <= 'z') val = c - 'a';
        else if (c >= '2' && c <= '7') val = c - '2' + 26;
        else if (c == '=') break;
        else continue;

        buffer = (buffer << 5) | val;
        bitsLeft += 5;
        if (bitsLeft >= 8)
        {
            result.push_back(static_cast<uint8_t>((buffer >> (bitsLeft - 8)) & 0xFF));
            bitsLeft -= 8;
        }
    }
    return result;
}

// 🔑 Base64 Encoder
std::string base64Encode(const uint8_t* data, size_t len)
{
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result = "";
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--)
    {
        char_array_3[i++] = *(data++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                result += chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            result += chars[char_array_4[j]];

        while ((i++ < 3))
            result += '=';
    }

    return result;
}

// 🔑 Base64 Decoder
std::vector<uint8_t> base64Decode(const std::string& input)
{
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<uint8_t> result;
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];

    while (in_ < input.size() && input[in_] != '=')
    {
        char_array_4[i++] = input[in_++];
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                result.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++)
            result.push_back(char_array_3[j]);
    }

    return result;
}

// 🔑 TOTP Rolling Generator (6 digits)
std::string generateTotp(const std::string& secret, uint64_t timeStep)
{
    auto key = base32Decode(secret);
    if (key.empty()) return "";

    uint8_t msg[8];
    for (int i = 7; i >= 0; i--)
    {
        msg[i] = static_cast<uint8_t>(timeStep & 0xFF);
        timeStep >>= 8;
    }

    uint8_t hash[20];
    hmacSha1(key.data(), key.size(), msg, 8, hash);

    int offset = hash[19] & 0xF;
    uint32_t binary = ((hash[offset] & 0x7F) << 24) |
                      ((hash[offset + 1] & 0xFF) << 16) |
                      ((hash[offset + 2] & 0xFF) << 8) |
                      (hash[offset + 3] & 0xFF);

    uint32_t otp = binary % 1000000;
    std::string result = std::to_string(otp);
    while (result.length() < 6)
    {
        result = "0" + result;
    }
    return result;
}
