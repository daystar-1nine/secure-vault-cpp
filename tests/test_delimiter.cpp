#include "../include/models/Credential.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

// Helper to print test results
void printResult(const std::string& testName, bool success) {
    if (success) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

int main() {
    std::cout << "Starting Delimiter Safety and Backward Compatibility Tests..." << std::endl;

    // --- TEST 1: Legacy Compatibility (Unescaped data) ---
    {
        // A typical legacy serialized line: site|user|pass|hist1;hist2|totp|hex_notes|attName|attData|category
        // Note: hex_notes is "my notes" in hex -> 6d79206e6f746573
        std::string legacyLine = "google.com|myuser|mypass|oldpass1;oldpass2|secret|6d79206e6f746573|file.txt|base64data|Login";
        Credential c = Credential::deserialize(legacyLine);

        bool ok = true;
        if (c.getWebsite() != "google.com") ok = false;
        if (c.getUsername() != "myuser") ok = false;
        if (c.getPassword() != "mypass") ok = false;
        if (c.getPasswordHistory().size() != 2) ok = false;
        if (c.getPasswordHistory()[0] != "oldpass1") ok = false;
        if (c.getPasswordHistory()[1] != "oldpass2") ok = false;
        if (c.getTotpSecret() != "secret") ok = false;
        if (c.getNotes() != "my notes") ok = false;
        if (c.getAttachmentName() != "file.txt") ok = false;
        if (c.getAttachmentData() != "base64data") ok = false;
        if (c.getCategory() != "Login") ok = false;

        printResult("Test 1: Legacy Compatibility (Unescaped Data)", ok);
    }

    // --- TEST 2: Escaping Delimiters ('|', ';', '\') ---
    {
        Credential original;
        original.setWebsite("site|with|pipes");
        original.setUsername("user\\with\\backslashes");
        original.setPassword("password;with;semicolons");
        original.addPasswordToHistory("old|pass1");
        original.addPasswordToHistory("old;pass2");
        original.addPasswordToHistory("old\\pass3");
        original.setTotpSecret("totp|secret;with\\both");
        original.setNotes("notes with | and ; and \\");
        original.setAttachmentName("attach|name;1\\2");
        original.setAttachmentData("attach\\data|3;4");
        original.setCategory("Category|Special");

        std::string serialized = original.serialize();
        std::cout << "Serialized string: " << serialized << std::endl;

        Credential deserialized = Credential::deserialize(serialized);

        bool ok = true;
        if (deserialized.getWebsite() != original.getWebsite()) {
            std::cout << "Website mismatch: " << deserialized.getWebsite() << " vs " << original.getWebsite() << std::endl;
            ok = false;
        }
        if (deserialized.getUsername() != original.getUsername()) {
            std::cout << "Username mismatch: " << deserialized.getUsername() << " vs " << original.getUsername() << std::endl;
            ok = false;
        }
        if (deserialized.getPassword() != original.getPassword()) {
            std::cout << "Password mismatch: " << deserialized.getPassword() << " vs " << original.getPassword() << std::endl;
            ok = false;
        }
        if (deserialized.getPasswordHistory().size() != original.getPasswordHistory().size()) {
            std::cout << "History size mismatch: " << deserialized.getPasswordHistory().size() << " vs " << original.getPasswordHistory().size() << std::endl;
            ok = false;
        } else {
            for (size_t i = 0; i < original.getPasswordHistory().size(); ++i) {
                if (deserialized.getPasswordHistory()[i] != original.getPasswordHistory()[i]) {
                    std::cout << "History[" << i << "] mismatch: " << deserialized.getPasswordHistory()[i] << " vs " << original.getPasswordHistory()[i] << std::endl;
                    ok = false;
                }
            }
        }
        if (deserialized.getTotpSecret() != original.getTotpSecret()) {
            std::cout << "TotpSecret mismatch: " << deserialized.getTotpSecret() << " vs " << original.getTotpSecret() << std::endl;
            ok = false;
        }
        if (deserialized.getNotes() != original.getNotes()) {
            std::cout << "Notes mismatch: " << deserialized.getNotes() << " vs " << original.getNotes() << std::endl;
            ok = false;
        }
        if (deserialized.getAttachmentName() != original.getAttachmentName()) {
            std::cout << "AttachmentName mismatch: " << deserialized.getAttachmentName() << " vs " << original.getAttachmentName() << std::endl;
            ok = false;
        }
        if (deserialized.getAttachmentData() != original.getAttachmentData()) {
            std::cout << "AttachmentData mismatch: " << deserialized.getAttachmentData() << " vs " << original.getAttachmentData() << std::endl;
            ok = false;
        }
        if (deserialized.getCategory() != original.getCategory()) {
            std::cout << "Category mismatch: " << deserialized.getCategory() << " vs " << original.getCategory() << std::endl;
            ok = false;
        }

        printResult("Test 2: Escaping Delimiters", ok);
    }

    // --- TEST 3: Empty Fields and Boundaries ---
    {
        Credential original("", "", "");
        std::string serialized = original.serialize();
        Credential deserialized = Credential::deserialize(serialized);

        bool ok = true;
        if (deserialized.getWebsite() != "") ok = false;
        if (deserialized.getUsername() != "") ok = false;
        if (deserialized.getPassword() != "") ok = false;
        if (deserialized.getCategory() != "Login") ok = false; // default category is Login if empty

        printResult("Test 3: Empty Fields and Boundaries", ok);
    }

    // --- TEST 4: Attachment Zero-IPC Optimization Emulation ---
    {
        std::string pendingAttachmentName = "";
        std::string pendingAttachmentData = "";

        // 1. Simulate attaching a file
        pendingAttachmentName = "myDoc.pdf";
        pendingAttachmentData = "dGhpcyBpcyBhIGZpbGU="; // base64 for "this is a file"

        // 2. Simulate api_add_credential saving a new credential
        std::string site = "github.com";
        std::string user = "coder";
        std::string pass = "safePass";
        std::string attName = "myDoc.pdf";

        // C++ resolves attachmentData
        std::string resolvedData = "";
        if (!attName.empty()) {
            if (attName == pendingAttachmentName && !pendingAttachmentData.empty()) {
                resolvedData = pendingAttachmentData;
                pendingAttachmentName = "";
                pendingAttachmentData = "";
            }
        }

        Credential cred(site, user, pass);
        cred.setAttachmentName(attName);
        cred.setAttachmentData(resolvedData);

        // Verify attachment saved
        bool ok = true;
        if (cred.getAttachmentName() != "myDoc.pdf") ok = false;
        if (cred.getAttachmentData() != "dGhpcyBpcyBhIGZpbGU=") ok = false;

        // 3. Simulate updating credential without changing attachment
        std::string newPass = "saferPass";
        std::string attNameUpdate = "myDoc.pdf";

        // C++ resolves:
        std::string resolvedDataUpdate = "";
        if (!attNameUpdate.empty()) {
            if (attNameUpdate == pendingAttachmentName && !pendingAttachmentData.empty()) {
                resolvedDataUpdate = pendingAttachmentData;
            } else {
                // look up existing
                if (cred.getWebsite() == site && cred.getUsername() == user) {
                    resolvedDataUpdate = cred.getAttachmentData();
                }
            }
        }

        Credential updatedCred(site, user, newPass);
        updatedCred.setAttachmentName(attNameUpdate);
        updatedCred.setAttachmentData(resolvedDataUpdate);

        // Verify attachment still preserved
        if (updatedCred.getAttachmentName() != "myDoc.pdf") ok = false;
        if (updatedCred.getAttachmentData() != "dGhpcyBpcyBhIGZpbGU=") ok = false;

        // 4. Simulate clearing attachment
        std::string attNameClear = "";
        std::string resolvedDataClear = ""; // since attNameClear is empty

        Credential clearedCred(site, user, newPass);
        clearedCred.setAttachmentName(attNameClear);
        clearedCred.setAttachmentData(resolvedDataClear);

        if (clearedCred.getAttachmentName() != "") ok = false;
        if (clearedCred.getAttachmentData() != "") ok = false;

        printResult("Test 4: Attachment Zero-IPC Optimization Emulation", ok);
    }

    std::cout << "Tests completed." << std::endl;
    return 0;
}
