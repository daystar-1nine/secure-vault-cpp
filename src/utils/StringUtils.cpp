#include "utils/StringUtils.h"
#include <fstream>
#include <sstream>

// 🔍 Escape special characters in strings for JSON serialization
std::string escapeJsonString(const std::string& input)
{
    std::string output = "";
    for (char c : input)
    {
        if (c == '"') output += "\\\"";
        else if (c == '\\') output += "\\\\";
        else if (c == '/') output += "\\/";
        else if (c == '\b') output += "\\b";
        else if (c == '\f') output += "\\f";
        else if (c == '\n') output += "\\n";
        else if (c == '\r') output += "\\r";
        else if (c == '\t') output += "\\t";
        else output += c;
    }
    return output;
}

// 📂 Escape special characters for CSV format
std::string escapeCsvString(const std::string& input)
{
    bool needQuotes = false;
    std::string output = "";
    for (char c : input)
    {
        if (c == '"')
        {
            output += "\"\"";
            needQuotes = true;
        }
        else
        {
            output += c;
            if (c == ',' || c == '\n' || c == '\r')
            {
                needQuotes = true;
            }
        }
    }
    if (needQuotes)
    {
        return "\"" + output + "\"";
    }
    return output;
}

// 📂 Stateful character-by-character CSV parser
std::vector<std::vector<std::string>> parseCsvFile(const std::string& filePath)
{
    std::vector<std::vector<std::string>> records;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return records;

    std::vector<std::string> currentRecord;
    std::string currentField = "";
    bool inQuotes = false;
    char c;

    while (file.get(c))
    {
        if (inQuotes)
        {
            if (c == '"')
            {
                char next;
                if (file.get(next))
                {
                    if (next == '"')
                    {
                        currentField += '"';
                    }
                    else
                    {
                        inQuotes = false;
                        // Put back the next char since it's not part of an escaped quote
                        file.putback(next);
                    }
                }
                else
                {
                    inQuotes = false;
                }
            }
            else
            {
                currentField += c;
            }
        }
        else
        {
            if (c == '"')
            {
                inQuotes = true;
            }
            else if (c == ',')
            {
                currentRecord.push_back(currentField);
                currentField = "";
            }
            else if (c == '\n')
            {
                currentRecord.push_back(currentField);
                records.push_back(currentRecord);
                currentRecord.clear();
                currentField = "";
            }
            else if (c == '\r')
            {
                // skip carriage return
            }
            else
            {
                currentField += c;
            }
        }
    }
    if (!currentField.empty() || !currentRecord.empty())
    {
        currentRecord.push_back(currentField);
        records.push_back(currentRecord);
    }
    return records;
}

// 🔍 Lightweight positional argument extractor from JSON array
std::string getArg(const std::string& req, size_t index)
{
    size_t pos = 0;
    for (size_t i = 0; i <= index; ++i)
    {
        pos = req.find("\"", pos);
        if (pos == std::string::npos) return "";
        size_t end = req.find("\"", pos + 1);
        while (end != std::string::npos && req[end - 1] == '\\') {
            end = req.find("\"", end + 1);
        }
        if (end == std::string::npos) return "";
        if (i == index) {
            std::string val = req.substr(pos + 1, end - pos - 1);
            std::string unescaped = "";
            for (size_t j = 0; j < val.length(); ++j) {
                if (val[j] == '\\' && j + 1 < val.length()) {
                    unescaped += val[j + 1];
                    j++;
                } else {
                    unescaped += val[j];
                }
            }
            return unescaped;
        }
        pos = end + 1;
    }
    return "";
}
