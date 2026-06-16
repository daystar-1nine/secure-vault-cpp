#ifndef RECOVERY_UTILS_H
#define RECOVERY_UTILS_H

#include <string>
#include <random>
#include <sstream>
#include <iomanip>

// Utility to generate a random hexadecimal recovery key of specified length (default 32 chars).
inline std::string generateRecoveryKey(size_t length = 32) {
    const char* hex_chars = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << hex_chars[dis(gen)];
    }
    return oss.str();
}

#endif // RECOVERY_UTILS_H
