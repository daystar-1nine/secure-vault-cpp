#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

std::string escapeJsonString(const std::string& input);
std::string escapeCsvString(const std::string& input);
std::vector<std::vector<std::string>> parseCsvFile(const std::string& filePath);
std::string getArg(const std::string& req, size_t index);

#endif
