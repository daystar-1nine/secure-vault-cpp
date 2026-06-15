#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>
#include <vector>
#include <cstdint>

std::vector<uint8_t> base32Decode(const std::string& input);
std::string base64Encode(const uint8_t* data, size_t len);
std::vector<uint8_t> base64Decode(const std::string& input);
std::string generateTotp(const std::string& secret, uint64_t timeStep);

#endif
