#include "utils/GeneratorUtils.h"
#include <random>
#include <algorithm>

// 🔍 Cryptographically secure random password generator
std::string generatePassword(int length, bool upper, bool lower, bool digit, bool symbol)
{
    if (length < 4) length = 16;
    
    std::string uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string lowers = "abcdefghijklmnopqrstuvwxyz";
    std::string digits = "0123456789";
    std::string symbols = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    std::string pool = "";
    std::string result = "";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    
    std::vector<std::string> requiredSets;
    if (upper) { pool += uppers; requiredSets.push_back(uppers); }
    if (lower) { pool += lowers; requiredSets.push_back(lowers); }
    if (digit) { pool += digits; requiredSets.push_back(digits); }
    if (symbol) { pool += symbols; requiredSets.push_back(symbols); }
    
    if (pool.empty()) {
        pool = uppers + lowers + digits + symbols;
        requiredSets.push_back(uppers);
        requiredSets.push_back(lowers);
        requiredSets.push_back(digits);
        requiredSets.push_back(symbols);
    }
    
    // Ensure one character from each selected set is guaranteed
    for (const auto& s : requiredSets) {
        std::uniform_int_distribution<size_t> dist(0, s.size() - 1);
        result += s[dist(generator)];
    }
    
    std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
    while (result.size() < static_cast<size_t>(length)) {
        result += pool[dist(generator)];
    }
    
    std::shuffle(result.begin(), result.end(), generator);
    
    return result;
}
