#include "auth/AuthManager.h"
#include "vault/VaultManager.h"
#include "storage/StorageManager.h"
#include "models/Credential.h"
#include "web/WebPortal.h"

#include <webview.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>

// 📂 Register shortcut in the Windows Start Menu on startup
static void createStartMenuShortcut()
{
    CoInitialize(NULL);
    IShellLink* pShellLink = NULL;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
    if (SUCCEEDED(hr))
    {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        pShellLink->SetPath(exePath);
        
        std::string exeStr(exePath);
        size_t lastSlash = exeStr.find_last_of("\\/");
        std::string dirStr = (lastSlash == std::string::npos) ? "" : exeStr.substr(0, lastSlash);
        pShellLink->SetWorkingDirectory(dirStr.c_str());
        pShellLink->SetDescription("Secure Password Vault");

        IPersistFile* pPersistFile = NULL;
        hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile);
        if (SUCCEEDED(hr))
        {
            char startMenuPath[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, startMenuPath)))
            {
                std::string shortcutFolder = std::string(startMenuPath) + "\\Microsoft\\Windows\\Start Menu\\Programs";
                std::string shortcutPath = shortcutFolder + "\\Secure Vault.lnk";
                
                wchar_t wShortcutPath[MAX_PATH];
                MultiByteToWideChar(CP_ACP, 0, shortcutPath.c_str(), -1, wShortcutPath, MAX_PATH);
                
                pPersistFile->Save(wShortcutPath, TRUE);
            }
            pPersistFile->Release();
        }
        pShellLink->Release();
    }
    CoUninitialize();
}
#endif

// 🔐 Secure Memory Wiper for C++ strings
static void secureWipeString(std::string& s)
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

// 🔍 Escape special characters in strings for JSON serialization
static std::string escapeJsonString(const std::string& input)
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

// 🔍 Lightweight positional argument extractor from JSON array
static std::string getArg(const std::string& req, size_t index)
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

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
#else
int main()
#endif
{
    try {
#ifdef _WIN32
        // Create Windows Start Menu Shortcut
        createStartMenuShortcut();
#endif

        // Managers
        AuthManager authManager;
        VaultManager vaultManager;
        StorageManager storageManager;

        std::string masterPassword = "";

        // Create desktop webview window (debug = true for devtools support)
        webview::webview w(true, nullptr);
        w.set_title("Secure Password Vault");
        w.set_size(1024, 768, WEBVIEW_HINT_NONE);

        // 🔹 Bind: Get Vault Status
        w.bind("api_status", [&](std::string /*req*/) -> std::string {
            bool exists = authManager.vaultExists();
            long long lockoutTime = authManager.getLockoutRemainingTime();
            
            std::string json = "{";
            json += "\"exists\":" + std::string(exists ? "true" : "false") + ",";
            json += "\"lockoutRemaining\":" + std::to_string(lockoutTime);
            json += "}";
            return json;
        });

        // 🔹 Bind: Load display scale settings
        w.bind("api_get_settings", [&](std::string /*req*/) -> std::string {
            float scale = 1.0f;
            std::ifstream file("data/settings.dat");
            if (file) {
                file >> scale;
            }
            if (scale < 0.8f || scale > 2.5f) scale = 1.0f;
            return "{\"scale\":" + std::to_string(scale) + "}";
        });

        // 🔹 Bind: Save display scale settings
        w.bind("api_save_settings", [&](std::string req) -> std::string {
            size_t start = req.find("[");
            size_t end = req.find("]");
            if (start != std::string::npos && end != std::string::npos && end > start + 1) {
                std::string scaleStr = req.substr(start + 1, end - start - 1);
                try {
                    float scale = std::stof(scaleStr);
                    std::filesystem::create_directories("data");
                    std::ofstream file("data/settings.dat");
                    if (file) {
                        file << scale;
                    }
                } catch (...) {}
            }
            return "{\"success\":true}";
        });

        // 🔹 Bind: Create Vault Master Password
        w.bind("api_setup", [&](std::string req) -> std::string {
            std::string pass = getArg(req, 0);
            std::string json;
            if (pass.empty()) {
                json = "{\"success\":false,\"error\":\"Password cannot be empty.\"}";
            } else {
                if (authManager.createVaultPassword(pass)) {
                    masterPassword = pass;
                    // Save empty vault to initialize
                    storageManager.saveVault(std::vector<Credential>(), masterPassword);
                    json = "{\"success\":true}";
                } else {
                    json = "{\"success\":false,\"error\":\"Failed to initialize vault config.\"}";
                }
            }
            secureWipeString(pass);
            return json;
        });

        // 🔹 Bind: Unlock Vault
        w.bind("api_login", [&](std::string req) -> std::string {
            std::string pass = getArg(req, 0);
            std::string json;
            if (authManager.verifyVaultPassword(pass)) {
                masterPassword = pass;
                auto loaded = storageManager.loadVault(masterPassword);
                vaultManager.clearVault();
                if (loaded) {
                    for (const auto& c : *loaded) {
                        vaultManager.addCredential(c);
                    }
                }
                json = "{\"success\":true}";
            } else {
                json = "{\"success\":false,\"error\":\"Invalid master password.\"}";
            }
            secureWipeString(pass);
            return json;
        });

        // 🔹 Bind: Retrieve Decrypted Credentials
        w.bind("api_get_credentials", [&](std::string /*req*/) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            const auto& credentials = vaultManager.getAllCredentials();
            std::string json = "{\"credentials\":[";
            for (size_t i = 0; i < credentials.size(); ++i) {
                const auto& c = credentials[i];
                json += "{";
                json += "\"website\":\"" + escapeJsonString(c.getWebsite()) + "\",";
                json += "\"username\":\"" + escapeJsonString(c.getUsername()) + "\",";
                json += "\"password\":\"" + escapeJsonString(c.getPassword()) + "\"";
                json += "}";
                if (i + 1 < credentials.size()) json += ",";
            }
            json += "]}";
            return json;
        });

        // 🔹 Bind: Add New Credential
        w.bind("api_add_credential", [&](std::string req) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string site = getArg(req, 0);
            std::string user = getArg(req, 1);
            std::string pass = getArg(req, 2);
            std::string json;
            if (site.empty() || user.empty() || pass.empty()) {
                json = "{\"success\":false,\"error\":\"All fields are required.\"}";
            } else {
                Credential credential(site, user, pass);
                if (vaultManager.addCredential(credential)) {
                    storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                    json = "{\"success\":true}";
                } else {
                    json = "{\"success\":false,\"error\":\"Credential already exists.\"}";
                }
            }
            secureWipeString(pass);
            return json;
        });

        // 🔹 Bind: Delete Credential
        w.bind("api_delete_credential", [&](std::string req) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string site = getArg(req, 0);
            std::string user = getArg(req, 1);
            std::string json;
            if (vaultManager.removeCredential(site, user)) {
                storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                json = "{\"success\":true}";
            } else {
                json = "{\"success\":false,\"error\":\"Credential not found.\"}";
            }
            return json;
        });

        // 🔹 Bind: Save Changes Explicitly
        w.bind("api_save", [&](std::string /*req*/) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string json;
            if (storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword)) {
                json = "{\"success\":true}";
            } else {
                json = "{\"success\":false,\"error\":\"Failed to save vault.\"}";
            }
            return json;
        });

        // 🔹 Bind: Lock Vault (Clear RAM keys)
        w.bind("api_lock", [&](std::string /*req*/) -> std::string {
            vaultManager.clearVault();
            secureWipeString(masterPassword);
            return "{\"success\":true}";
        });

        // 🔹 Bind: Verify master password
        w.bind("api_verify_password", [&](std::string req) -> std::string {
            std::string pass = getArg(req, 0);
            bool verified = authManager.verifyVaultPassword(pass);
            secureWipeString(pass);
            return verified ? "{\"success\":true}" : "{\"success\":false}";
        });

        // 🔹 Bind: Shutdown App (Close Window)
        w.bind("api_shutdown", [&](std::string /*req*/) -> std::string {
            vaultManager.clearVault();
            secureWipeString(masterPassword);
            w.terminate();
            return "{\"success\":true}";
        });

        // Load HTML content and start window loop
        w.set_html(INDEX_HTML_CONTENT);
        w.run();

        return 0;
    } catch (const std::exception& e) {
#ifdef _WIN32
        MessageBoxA(NULL, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
#else
        std::cerr << "Fatal Exception: " << e.what() << std::endl;
#endif
        return 1;
    } catch (...) {
#ifdef _WIN32
        MessageBoxA(NULL, "An unknown error occurred.", "Fatal Error", MB_OK | MB_ICONERROR);
#else
        std::cerr << "Fatal Unknown Exception" << std::endl;
#endif
        return 1;
    }
}