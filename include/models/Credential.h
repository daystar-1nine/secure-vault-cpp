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
        : website(site), username(user), password(pass) {}

    // 🔹 Getters (read-only access)
    std::string getWebsite() const { return website; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::vector<std::string> getPasswordHistory() const { return passwordHistory; }
    std::string getTotpSecret() const { return totpSecret; }
    std::string getNotes() const { return notes; }
    std::string getAttachmentName() const { return attachmentName; }
    std::string getAttachmentData() const { return attachmentData; }

    // 🔹 Setters
    void setWebsite(const std::string& site) { website = site; }
    void setUsername(const std::string& user) { username = user; }
    void setPassword(const std::string& pass) { password = pass; }
    void setTotpSecret(const std::string& val) { totpSecret = val; }
    void setNotes(const std::string& val) { notes = val; }
    void setAttachmentName(const std::string& val) { attachmentName = val; }
    void setAttachmentData(const std::string& val) { attachmentData = val; }

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

    // 🔹 Convert to string for file storage
    std::string serialize() const
    {
        std::string historyStr = "";
        for (size_t i = 0; i < passwordHistory.size(); ++i)
        {
            historyStr += passwordHistory[i];
            if (i + 1 < passwordHistory.size()) historyStr += ";";
        }
        return website + "|" + username + "|" + password + "|" + historyStr + "|" + totpSecret + "|" + toHex(notes) + "|" + attachmentName + "|" + attachmentData;
    }

    // 🔹 Load from string (reverse of serialize)
    static Credential deserialize(const std::string& data)
    {
        std::stringstream ss(data);
        std::string site, user, pass, historyStr, totp, notesHex, attName, attData;

        std::getline(ss, site, '|');
        std::getline(ss, user, '|');
        std::getline(ss, pass, '|');
        std::getline(ss, historyStr, '|');
        std::getline(ss, totp, '|');
        std::getline(ss, notesHex, '|');
        std::getline(ss, attName, '|');
        std::getline(ss, attData, '|');

        Credential c(site, user, pass);
        if (!historyStr.empty())
        {
            std::stringstream hss(historyStr);
            std::string oldPass;
            while (std::getline(hss, oldPass, ';'))
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
        return c;
    }

    // 🔹 Clear sensitive data from memory (wipes current and historical passwords)
    void clearSensitiveData()
    {
        secureWipe(password);
        secureWipe(totpSecret);
        secureWipe(notes);
        secureWipe(attachmentData);
        for (auto& oldPass : passwordHistory)
        {
            secureWipe(oldPass);
        }
        passwordHistory.clear();
    }
};

#endif