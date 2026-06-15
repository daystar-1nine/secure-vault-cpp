#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

/*
    Credential class represents a single entry
    in the password vault.

    Design goals:
    - Encapsulation (data hidden)
    - Easy serialization (for file storage)
    - Security awareness (clear sensitive data)
*/
class Credential
{
private:
    std::string website;
    std::string username;
    std::string password;
    std::vector<std::string> passwordHistory;
    std::string totpSecret;
    std::string notes;
    std::string attachmentName;
    std::string attachmentData;
    std::string category;
    std::string expiryDate;  // YYYY-MM-DD format, empty = never expires

    // Hex helpers to serialize notes safely
    static std::string toHex(const std::string& input)
    {
        std::string output = "";
        static const char hex_chars[] = "0123456789abcdef";
        for (unsigned char c : input)
        {
            output += hex_chars[c >> 4];
            output += hex_chars[c & 0x0F];
        }
        return output;
    }

    static std::string fromHex(const std::string& hex)
    {
        std::string output = "";
        for (size_t i = 0; i < hex.length(); i += 2)
        {
            if (i + 1 >= hex.length()) break;
            try {
                std::string byteString = hex.substr(i, 2);
                char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
                output += byte;
            } catch (...) {}
        }
        return output;
    }

    // Delimiter escaping helpers to prevent DB corruption
    static std::string escape(const std::string& input, char delimiter)
    {
        std::string output = "";
        for (char c : input)
        {
            if (c == '\\') output += "\\\\";
            else if (c == delimiter) {
                output += '\\';
                output += delimiter;
            }
            else output += c;
        }
        return output;
    }

    static std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token = "";
        for (size_t i = 0; i < s.length(); ++i)
        {
            if (s[i] == '\\' && i + 1 < s.length() && s[i + 1] == delimiter)
            {
                token += delimiter;
                i++;
            }
            else if (s[i] == '\\' && i + 1 < s.length() && s[i + 1] == '\\')
            {
                token += '\\';
                i++;
            }
            else if (s[i] == delimiter)
            {
                tokens.push_back(token);
                token = "";
            }
            else
            {
                token += s[i];
            }
        }
        tokens.push_back(token);
        return tokens;
    }

    void secureWipe(std::string& s)
    {
        if (s.empty()) return;
        volatile char* p = const_cast<volatile char*>(s.data());
        size_t size = s.size();
        while (size--)
        {
            *p++ = 0;
        }
        s.clear();
    }

public:
    // 🔹 Constructors
    Credential() = default;

    Credential(const std::string& site,
               const std::string& user,
               const std::string& pass)
        : website(site), username(user), password(pass), category("Login") {}

    // 🔹 Getters (read-only access)
    std::string getWebsite() const { return website; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::vector<std::string> getPasswordHistory() const { return passwordHistory; }
    std::string getTotpSecret() const { return totpSecret; }
    std::string getNotes() const { return notes; }
    std::string getAttachmentName() const { return attachmentName; }
    std::string getAttachmentData() const { return attachmentData; }
    std::string getCategory() const { return category; }
    std::string getExpiryDate() const { return expiryDate; }

    // 🔹 Setters
    void setWebsite(const std::string& site) { website = site; }
    void setUsername(const std::string& user) { username = user; }
    void setPassword(const std::string& pass) { password = pass; }
    void setTotpSecret(const std::string& val) { totpSecret = val; }
    void setNotes(const std::string& val) { notes = val; }
    void setAttachmentName(const std::string& val) { attachmentName = val; }
    void setAttachmentData(const std::string& val) { attachmentData = val; }
    void setCategory(const std::string& val) { category = val; }
    void setExpiryDate(const std::string& val) { expiryDate = val; }

    // 🔹 Password history helper
    void addPasswordToHistory(const std::string& pass)
    {
        if (pass.empty()) return;
        // Prevent back-to-back duplicates in history
        if (!passwordHistory.empty() && passwordHistory.back() == pass) return;

        passwordHistory.push_back(pass);
        if (passwordHistory.size() > 5)
        {
            passwordHistory.erase(passwordHistory.begin());
        }
    }

    // 🔹 Convert to string for file storage (safely escapes delimiters)
    std::string serialize() const
    {
        std::string historyStr = "";
        for (size_t i = 0; i < passwordHistory.size(); ++i)
        {
            historyStr += escape(passwordHistory[i], ';');
            if (i + 1 < passwordHistory.size()) historyStr += ";";
        }
        return escape(website, '|') + "|" +
               escape(username, '|') + "|" +
               escape(password, '|') + "|" +
               escape(historyStr, '|') + "|" +
               escape(totpSecret, '|') + "|" +
               toHex(notes) + "|" +
               escape(attachmentName, '|') + "|" +
               escape(attachmentData, '|') + "|" +
               escape(category, '|') + "|" +
               escape(expiryDate, '|');
    }

    // 🔹 Load from string (safely splits escaped fields)
    static Credential deserialize(const std::string& data)
    {
        std::vector<std::string> parts = split(data, '|');

        std::string site = parts.size() > 0 ? parts[0] : "";
        std::string user = parts.size() > 1 ? parts[1] : "";
        std::string pass = parts.size() > 2 ? parts[2] : "";
        std::string historyStr = parts.size() > 3 ? parts[3] : "";
        std::string totp = parts.size() > 4 ? parts[4] : "";
        std::string notesHex = parts.size() > 5 ? parts[5] : "";
        std::string attName = parts.size() > 6 ? parts[6] : "";
        std::string attData = parts.size() > 7 ? parts[7] : "";
        std::string cat = parts.size() > 8 ? parts[8] : "";

        Credential c(site, user, pass);
        if (!historyStr.empty())
        {
            std::vector<std::string> historyParts = split(historyStr, ';');
            for (const auto& oldPass : historyParts)
            {
                if (!oldPass.empty())
                {
                    c.passwordHistory.push_back(oldPass);
                }
            }
        }
        c.totpSecret = totp;
        c.notes = fromHex(notesHex);
        c.attachmentName = attName;
        c.attachmentData = attData;
        c.category = cat.empty() ? "Login" : cat;
        c.expiryDate = parts.size() > 9 ? parts[9] : "";
        return c;
    }

    // 🔹 Clear sensitive data from memory (wipes current and historical passwords)
    void clearSensitiveData()
    {
        secureWipe(password);
        secureWipe(totpSecret);
        secureWipe(notes);
        secureWipe(attachmentData);
        secureWipe(category);
        for (auto& oldPass : passwordHistory)
        {
            secureWipe(oldPass);
        }
        passwordHistory.clear();
    }
};

#endif