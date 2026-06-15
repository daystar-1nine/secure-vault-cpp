#ifndef GENERATOR_UTILS_H
#define GENERATOR_UTILS_H

#include <string>

std::string generatePassword(int length, bool upper, bool lower, bool digit, bool symbol);
std::string generatePassphrase(int wordCount, const std::string& separator);

#endif
