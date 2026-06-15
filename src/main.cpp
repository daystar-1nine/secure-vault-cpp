#include "auth/AuthManager.h" // rebuild trigger 3
#include "vault/VaultManager.h"
#include "storage/StorageManager.h"
#include "models/Credential.h"
#include "web/WebPortal.h"
#include "utils/SecurityUtils.h"
#include "utils/CryptoUtils.h"
#include "utils/SystemUtils.h"
#include "utils/StringUtils.h"
#include "utils/GeneratorUtils.h"

#include <webview.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <random>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <commctrl.h>
#include <commdlg.h>

// 🕒 Subclassing window procedure and wtsapi functions for Auto-Lock
typedef BOOL (WINAPI *WTSRegisterSessionNotificationProc)(HWND, DWORD);
typedef BOOL (WINAPI *WTSUnRegisterSessionNotificationProc)(HWND);

static WTSRegisterSessionNotificationProc pWTSRegisterSessionNotification = nullptr;
static WTSUnRegisterSessionNotificationProc pWTSUnRegisterSessionNotification = nullptr;

static void loadWtsApi()
{
    HMODULE hWts = LoadLibraryA("wtsapi32.dll");
    if (hWts)
    {
        pWTSRegisterSessionNotification = (WTSRegisterSessionNotificationProc)GetProcAddress(hWts, "WTSRegisterSessionNotification");
        pWTSUnRegisterSessionNotification = (WTSUnRegisterSessionNotificationProc)GetProcAddress(hWts, "WTSUnRegisterSessionNotification");
    }
}

// Subclass window procedure
static LRESULT CALLBACK WebviewWindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    webview::webview* w = reinterpret_cast<webview::webview*>(dwRefData);
    if (uMsg == 0x02B1 /* WM_WTSSESSION_CHANGE */)
    {
        if (wParam == 2 /* WTS_SESSION_LOCK */)
        {
            w->eval("handleLock();");
        }
    }
    else if (uMsg == 0x011F /* WM_POWERBROADCAST */)
    {
        if (wParam == 4 /* PBT_APMSUSPEND */)
        {
            w->eval("handleLock();");
        }
    }
    else if (uMsg == 0x0002 /* WM_DESTROY */)
    {
        if (pWTSUnRegisterSessionNotification)
        {
            pWTSUnRegisterSessionNotification(hWnd);
        }
        RemoveWindowSubclass(hWnd, WebviewWindowSubclass, uIdSubclass);
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
#endif


// Utility helper functions have been moved to include/utils and src/utils.

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

        // Create desktop webview window (debug = true in debug builds, false in release builds)
#ifdef NDEBUG
        webview::webview w(false, nullptr);
#else
        webview::webview w(true, nullptr);
#endif
        w.set_title("Secure Password Vault");
        w.set_size(1024, 768, WEBVIEW_HINT_NONE);

#ifdef _WIN32
        HWND hwnd = reinterpret_cast<HWND>(w.window().value());
        if (hwnd)
        {
            loadWtsApi();
            if (pWTSRegisterSessionNotification)
            {
                pWTSRegisterSessionNotification(hwnd, 0 /* NOTIFY_FOR_THIS_SESSION */);
            }
            SetWindowSubclass(hwnd, WebviewWindowSubclass, 1, reinterpret_cast<DWORD_PTR>(&w));
        }
#endif

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



        // 🔹 Bind: Create Vault Master Password
        w.bind("api_setup", [&](std::string req) -> std::string {
            std::string pass = getArg(req, 0);
            secureLockString(pass);
            std::string json;
            if (pass.empty()) {
                json = "{\"success\":false,\"error\":\"Password cannot be empty.\"}";
            } else {
                if (authManager.createVaultPassword(pass)) {
                    masterPassword = pass;
                    secureLockString(masterPassword);
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
            secureLockString(pass);
            std::string json;
            if (authManager.verifyVaultPassword(pass)) {
                masterPassword = pass;
                secureLockString(masterPassword);
                auto loaded = storageManager.loadVault(masterPassword);
                vaultManager.clearVault();
                if (loaded) {
                    for (const auto& c : *loaded) {
                        vaultManager.addCredential(c);
                    }
                }
                json = "{\"success\":true}";
            } else {
                if (!authManager.vaultExists()) {
                    json = "{\"success\":false,\"destructed\":true,\"error\":\"Vault has been permanently self-destructed due to 10 consecutive failed attempts!\"}";
                } else {
                    json = "{\"success\":false,\"error\":\"Invalid master password.\"}";
                }
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
            
            // Calculate current TOTP counter and remaining time
            uint64_t nowSecs = static_cast<uint64_t>(time(NULL));
            uint64_t timeStep = nowSecs / 30;
            int totpRemaining = 30 - (nowSecs % 30);

            for (size_t i = 0; i < credentials.size(); ++i) {
                const auto& c = credentials[i];
                json += "{";
                json += "\"website\":\"" + escapeJsonString(c.getWebsite()) + "\",";
                json += "\"username\":\"" + escapeJsonString(c.getUsername()) + "\",";
                json += "\"password\":\"" + escapeJsonString(c.getPassword()) + "\",";
                
                // TOTP rolling code
                std::string totpCode = "";
                if (!c.getTotpSecret().empty()) {
                    totpCode = generateTotp(c.getTotpSecret(), timeStep);
                }
                json += "\"totp\":\"" + totpCode + "\",";
                json += "\"totp_remaining\":" + std::to_string(totpRemaining) + ",";
                json += "\"totp_secret\":\"" + escapeJsonString(c.getTotpSecret()) + "\",";
                
                // Notes, attachmentName, attachmentData
                json += "\"notes\":\"" + escapeJsonString(c.getNotes()) + "\",";
                json += "\"attachmentName\":\"" + escapeJsonString(c.getAttachmentName()) + "\",";
                json += "\"attachmentData\":\"" + c.getAttachmentData() + "\","; // Base64 data doesn't need escaping
                json += "\"category\":\"" + escapeJsonString(c.getCategory()) + "\",";

                json += "\"history\":[";
                const auto& history = c.getPasswordHistory();
                for (size_t hIdx = 0; hIdx < history.size(); ++hIdx) {
                    json += "\"" + escapeJsonString(history[hIdx]) + "\"";
                    if (hIdx + 1 < history.size()) json += ",";
                }
                json += "]";
                json += "}";
                if (i + 1 < credentials.size()) json += ",";
            }
            json += "]}";
            return json;
        });

        // 🔹 Bind: Capture JS Console Logs
        w.bind("api_console_log", [&](std::string req) -> std::string {
            {
                std::ofstream log("data/api_debug.log", std::ios::app);
                log << "JS Log: " << getArg(req, 0) << "\n";
            }
            return "{}";
        });

        // 🔹 Bind: Get Theme (plain text, default: zinc)
        w.bind("api_get_theme", [&](std::string /*req*/) -> std::string {
            std::ifstream file("data/user_theme.dat");
            if (!file) {
                return "\"zinc\"";
            }
            std::string theme;
            std::getline(file, theme);
            if (theme.empty()) theme = "zinc";
            return "\"" + theme + "\"";
        });

        // 🔹 Bind: Set Theme (plain text)
        w.bind("api_set_theme", [&](std::string req) -> std::string {
            std::string theme = getArg(req, 0);
            std::ofstream file("data/user_theme.dat");
            if (file) {
                file << theme;
                return "{\"success\":true}";
            }
            return "{\"success\":false}";
        });

        // 🔹 Bind: Get settings (encrypted with master password)
        w.bind("api_get_settings", [&](std::string /*req*/) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::ifstream file("data/user_settings.dat", std::ios::binary);
            if (!file) {
                return "{}";
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            Encryption encryption;
            auto decrypted = encryption.decrypt(buffer.str(), masterPassword);
            if (!decrypted) {
                return "{}";
            }
            return *decrypted;
        });

        // 🔹 Bind: Save settings (encrypted with master password)
        w.bind("api_save_settings", [&](std::string req) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string settingsJson = getArg(req, 0);
            secureLockString(settingsJson);

            Encryption encryption;
            std::string ciphertext = encryption.encrypt(settingsJson, masterPassword);
            secureWipeString(settingsJson);

            std::ofstream file("data/user_settings.dat", std::ios::binary);
            if (file) {
                file.write(ciphertext.data(), ciphertext.size());
                file.close();
                return "{\"success\":true}";
            }
            return "{\"success\":false}";
        });


        // 🔹 Bind: Add / Update Credential (archives old password to history)
        w.bind("api_add_credential", [&](std::string req) -> std::string {
            {
                std::ofstream log("data/api_debug.log", std::ios::app);
                log << "req: " << req << "\n";
            }
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string site = getArg(req, 0);
            std::string user = getArg(req, 1);
            std::string pass = getArg(req, 2);
            secureLockString(pass);
            std::string totpSecret = getArg(req, 3);
            secureLockString(totpSecret);
            std::string notes = getArg(req, 4);
            secureLockString(notes);
            std::string attachmentName = getArg(req, 5);
            std::string attachmentData = getArg(req, 6);
            secureLockString(attachmentData);
            std::string category = getArg(req, 7);
            if (category.empty()) category = "Login";
            secureLockString(category);

            std::string json;
            if (site.empty() || user.empty() || pass.empty()) {
                json = "{\"success\":false,\"error\":\"All fields are required.\"}";
            } else {
                Credential credential(site, user, pass);
                credential.setTotpSecret(totpSecret);
                credential.setNotes(notes);
                credential.setAttachmentName(attachmentName);
                credential.setAttachmentData(attachmentData);
                credential.setCategory(category);

                if (vaultManager.addOrUpdateCredential(credential)) {
                    storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                    json = "{\"success\":true}";
                } else {
                    json = "{\"success\":false,\"error\":\"Failed to save credential.\"}";
                }
            }
            secureWipeString(pass);
            secureWipeString(totpSecret);
            secureWipeString(notes);
            secureWipeString(attachmentData);
            secureWipeString(category);
            return json;
        });

        // 🔹 Bind: Clear clipboard conditionally if matching expected password
        w.bind("api_clear_clipboard_if_matching", [&](std::string req) -> std::string {
            std::string expected = getArg(req, 0);
            secureLockString(expected);
            bool cleared = false;
#ifdef _WIN32
            cleared = clearClipboardIfMatching(expected);
#endif
            secureWipeString(expected);
            return cleared ? "{\"success\":true}" : "{\"success\":false}";
        });

        // 🔹 Bind: Attach file (opens native Win32 dialog, reads up to 2MB, returns base64)
        w.bind("api_attach_file", [&](std::string /*req*/) -> std::string {
            if (masterPassword.empty()) return "{\"error\":\"Unauthorized\"}";
            std::string json;
#ifdef _WIN32
            HWND hwnd = reinterpret_cast<HWND>(w.window().value());
            char szFile[260] = {0};
            OPENFILENAMEA ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            
            if (GetOpenFileNameA(&ofn) == TRUE) {
                std::string filePath(ofn.lpstrFile);
                std::ifstream file(filePath, std::ios::binary);
                if (file) {
                    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();

                    if (buffer.size() > 2 * 1024 * 1024) {
                        json = "{\"success\":false,\"error\":\"File size exceeds the 2MB limit.\"}";
                    } else {
                        std::string base64Data = base64Encode(buffer.data(), buffer.size());
                        std::filesystem::path p(filePath);
                        json = "{";
                        json += "\"success\":true,";
                        json += "\"fileName\":\"" + escapeJsonString(p.filename().string()) + "\",";
                        json += "\"fileData\":\"" + base64Data + "\"";
                        json += "}";
                    }
                } else {
                    json = "{\"success\":false,\"error\":\"Failed to read file.\"}";
                }
            } else {
                json = "{\"success\":false,\"error\":\"Cancelled\"}";
            }
#else
            json = "{\"success\":false,\"error\":\"Not supported on this platform\"}";
#endif
            return json;
        });

        // 🔹 Bind: Download file (opens native Win32 dialog, decodes base64, writes to disk)
        w.bind("api_download_file", [&](std::string req) -> std::string {
            if (masterPassword.empty()) return "{\"error\":\"Unauthorized\"}";
            std::string fileName = getArg(req, 0);
            std::string base64Data = getArg(req, 1);
            secureLockString(base64Data);
            std::string json;

#ifdef _WIN32
            HWND hwnd = reinterpret_cast<HWND>(w.window().value());
            char szFile[260] = {0};
            strncpy(szFile, fileName.c_str(), sizeof(szFile) - 1);

            OPENFILENAMEA ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

            if (GetSaveFileNameA(&ofn) == TRUE) {
                std::string savePath(ofn.lpstrFile);
                std::ofstream file(savePath, std::ios::binary);
                if (file) {
                    auto bytes = base64Decode(base64Data);
                    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
                    file.close();
                    json = "{\"success\":true,\"path\":\"" + escapeJsonString(savePath) + "\"}";
                } else {
                    json = "{\"success\":false,\"error\":\"Failed to write file.\"}";
                }
            } else {
                json = "{\"success\":false,\"error\":\"Cancelled\"}";
            }
#else
            json = "{\"success\":false,\"error\":\"Not supported on this platform\"}";
#endif
            secureWipeString(base64Data);
            return json;
        });

        // 🔹 Bind: Get system-wide idle time
        w.bind("api_get_system_idle_time", [&](std::string /*req*/) -> std::string {
            uint32_t idleMs = 0;
#ifdef _WIN32
            idleMs = getSystemIdleTimeMs();
#endif
            return "{\"idleMs\":" + std::to_string(idleMs) + "}";
        });

        // 🔹 Bind: Export CSV (native dialog + master password verification)
        w.bind("api_export_csv", [&](std::string req) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }
            std::string pass = getArg(req, 0);
            secureLockString(pass);
            if (!authManager.verifyVaultPassword(pass)) {
                secureWipeString(pass);
                return "{\"success\":false,\"error\":\"Invalid master password.\"}";
            }
            secureWipeString(pass);

#ifdef _WIN32
            HWND hwnd = reinterpret_cast<HWND>(w.window().value());
            std::string filePath = saveFileDialog(hwnd);
#else
            std::string filePath = "";
#endif
            if (filePath.empty()) {
                return "{\"success\":false,\"error\":\"No file selected.\"}";
            }

            std::ofstream file(filePath, std::ios::binary);
            if (!file) {
                return "{\"success\":false,\"error\":\"Failed to open file for writing.\"}";
            }

            // Write BOM for Excel UTF-8 compatibility
            file << "\xEF\xBB\xBF";
            // Header
            file << "website,username,password,history\n";

            for (const auto& c : vaultManager.getAllCredentials()) {
                file << escapeCsvString(c.getWebsite()) << ","
                     << escapeCsvString(c.getUsername()) << ","
                     << escapeCsvString(c.getPassword()) << ",";
                
                std::string histStr = "";
                const auto& history = c.getPasswordHistory();
                for (size_t i = 0; i < history.size(); ++i) {
                    histStr += history[i];
                    if (i + 1 < history.size()) histStr += ";";
                }
                file << escapeCsvString(histStr) << "\n";
            }
            file.close();
            return "{\"success\":true,\"path\":\"" + escapeJsonString(filePath) + "\"}";
        });

        // 🔹 Bind: Import CSV (native dialog + automatic merge/update)
        w.bind("api_import_csv", [&](std::string /*req*/) -> std::string {
            if (masterPassword.empty()) {
                return "{\"error\":\"Unauthorized\"}";
            }

#ifdef _WIN32
            HWND hwnd = reinterpret_cast<HWND>(w.window().value());
            std::string filePath = openFileDialog(hwnd);
#else
            std::string filePath = "";
#endif
            if (filePath.empty()) {
                return "{\"success\":false,\"error\":\"No file selected.\"}";
            }

            auto records = parseCsvFile(filePath);
            if (records.empty()) {
                return "{\"success\":false,\"error\":\"No records found or file could not be read.\"}";
            }

            size_t siteCol = 0, userCol = 1, passCol = 2, histCol = 3;
            bool hasHeader = false;

            const auto& firstRow = records[0];
            bool foundAny = false;
            for (size_t i = 0; i < firstRow.size(); ++i) {
                std::string col = firstRow[i];
                std::transform(col.begin(), col.end(), col.begin(), ::tolower);
                if (col == "website" || col == "url" || col == "site" || col == "title") {
                    siteCol = i;
                    foundAny = true;
                } else if (col == "username" || col == "user" || col == "email" || col == "login") {
                    userCol = i;
                    foundAny = true;
                } else if (col == "password" || col == "pass") {
                    passCol = i;
                    foundAny = true;
                } else if (col == "history" || col == "passwordhistory") {
                    histCol = i;
                    foundAny = true;
                }
            }
            if (foundAny) {
                hasHeader = true;
            }

            size_t startRow = hasHeader ? 1 : 0;
            int importedCount = 0;
            for (size_t r = startRow; r < records.size(); ++r) {
                const auto& row = records[r];
                if (row.size() <= std::max({siteCol, userCol, passCol})) continue;

                std::string site = row[siteCol];
                std::string user = row[userCol];
                std::string pass = row[passCol];

                if (site.empty() || user.empty() || pass.empty()) continue;

                Credential cred(site, user, pass);
                if (row.size() > histCol) {
                    std::string histStr = row[histCol];
                    if (!histStr.empty()) {
                        std::stringstream hss(histStr);
                        std::string oldPass;
                        while (std::getline(hss, oldPass, ';')) {
                            if (!oldPass.empty()) {
                                cred.addPasswordToHistory(oldPass);
                            }
                        }
                    }
                }
                
                vaultManager.addOrUpdateCredential(cred);
                importedCount++;
            }

            if (importedCount > 0) {
                storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
            }

            return "{\"success\":true,\"imported\":" + std::to_string(importedCount) + "}";
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
            secureLockString(pass);
            bool verified = authManager.verifyVaultPassword(pass);
            secureWipeString(pass);
            return verified ? "{\"success\":true}" : "{\"success\":false}";
        });

        // 🔹 Bind: Cryptographically secure password generator
        w.bind("api_generate_password", [&](std::string req) -> std::string {
            int length = std::stoi(getArg(req, 0));
            bool upper = getArg(req, 1) == "true";
            bool lower = getArg(req, 2) == "true";
            bool digit = getArg(req, 3) == "true";
            bool symbol = getArg(req, 4) == "true";
            
            std::string password = generatePassword(length, upper, lower, digit, symbol);
            secureLockString(password);
            std::string json = "{\"password\":\"" + escapeJsonString(password) + "\"}";
            secureWipeString(password);
            return json;
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