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

// Native Win32 file dialogs
static std::string openFileDialog(HWND hwnd)
{
    char szFile[260] = {0};
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn) == TRUE) {
        return std::string(ofn.lpstrFile);
    }
    return "";
}

static std::string saveFileDialog(HWND hwnd)
{
    char szFile[260] = {0};
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameA(&ofn) == TRUE) {
        std::string path = ofn.lpstrFile;
        if (path.length() < 4 || path.substr(path.length() - 4) != ".csv") {
            path += ".csv";
        }
        return path;
    }
    return "";
}

// 🕒 Check system idle time (inactivity)
static uint32_t getSystemIdleTimeMs()
{
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lii))
    {
        DWORD now = GetTickCount();
        return now - lii.dwTime;
    }
    return 0;
}

// 📋 Conditional clipboard clearing
static bool clearClipboardIfMatching(const std::string& expected)
{
    if (!OpenClipboard(NULL)) return false;
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData)
    {
        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText)
        {
            std::string currentText(pszText);
            GlobalUnlock(hData);
            if (currentText == expected)
            {
                EmptyClipboard();
                CloseClipboard();
                return true;
            }
        }
    }
    CloseClipboard();
    return false;
}
#endif

// 🔐 Secure Memory Wiper for C++ strings
static void secureWipeString(std::string& s)
{
    if (s.empty()) return;
#ifdef _WIN32
    VirtualUnlock(const_cast<char*>(s.data()), s.size());
#endif
    volatile char* p = const_cast<volatile char*>(s.data());
    size_t size = s.size();
    while (size--)
    {
        *p++ = 0;
    }
    s.clear();
}

// 🔒 Memory Locker for sensitive strings (prevents paging to disk)
static void secureLockString(std::string& s)
{
    if (s.empty()) return;
#ifdef _WIN32
    VirtualLock(const_cast<char*>(s.data()), s.size());
#endif
}

// 🔑 Base32 Decoder for TOTP Secrets
static std::vector<uint8_t> base32Decode(const std::string& input)
{
    std::vector<uint8_t> result;
    int buffer = 0;
    int bitsLeft = 0;
    for (char c : input)
    {
        if (c == ' ' || c == '-') continue;
        int val = 0;
        if (c >= 'A' && c <= 'Z') val = c - 'A';
        else if (c >= 'a' && c <= 'z') val = c - 'a';
        else if (c >= '2' && c <= '7') val = c - '2' + 26;
        else if (c == '=') break;
        else continue;

        buffer = (buffer << 5) | val;
        bitsLeft += 5;
        if (bitsLeft >= 8)
        {
            result.push_back(static_cast<uint8_t>((buffer >> (bitsLeft - 8)) & 0xFF));
            bitsLeft -= 8;
        }
    }
    return result;
}

// 🔑 Self-contained SHA-1 implementation
struct SHA1
{
    uint32_t state[5];
    uint32_t count[2];
    uint8_t buffer[64];

    void init()
    {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
        state[4] = 0xC3D2E1F0;
        count[0] = count[1] = 0;
    }

    static uint32_t rol(uint32_t value, uint32_t bits)
    {
        return (value << bits) | (value >> (32 - bits));
    }

    void transform(const uint8_t* block)
    {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
        {
            w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | block[i * 4 + 3];
        }
        for (int i = 16; i < 80; i++)
        {
            w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

        for (int i = 0; i < 20; i++)
        {
            uint32_t temp = rol(a, 5) + ((b & c) | (~b & d)) + e + w[i] + 0x5A827999;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 20; i < 40; i++)
        {
            uint32_t temp = rol(a, 5) + (b ^ c ^ d) + e + w[i] + 0x6ED9EBA1;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 40; i < 60; i++)
        {
            uint32_t temp = rol(a, 5) + ((b & c) | (b & d) | (c & d)) + e + w[i] + 0x8F1BBCDC;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }
        for (int i = 60; i < 80; i++)
        {
            uint32_t temp = rol(a, 5) + (b ^ c ^ d) + e + w[i] + 0xCA62C1D6;
            e = d; d = c; c = rol(b, 30); b = a; a = temp;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
    }

    void update(const uint8_t* data, uint32_t len)
    {
        uint32_t j = (count[0] >> 3) & 63;
        if ((count[0] += len << 3) < (len << 3)) count[1]++;
        count[1] += (len >> 29);
        uint32_t i = 0;
        if ((j + len) > 63)
        {
            memcpy(&buffer[j], data, (i = 64 - j));
            transform(buffer);
            for (; i + 63 < len; i += 64)
            {
                transform(&data[i]);
            }
            j = 0;
        }
        memcpy(&buffer[j], &data[i], len - i);
    }

    void final(uint8_t digest[20])
    {
        uint8_t finalcount[8];
        for (int i = 0; i < 8; i++)
        {
            finalcount[i] = static_cast<uint8_t>((count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);
        }
        uint8_t c = 0200;
        update(&c, 1);
        while ((count[0] & 504) != 448)
        {
            uint8_t z = 0;
            update(&z, 1);
        }
        update(finalcount, 8);
        for (int i = 0; i < 20; i++)
        {
            digest[i] = static_cast<uint8_t>((state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
        }
    }
};

// 🔑 HMAC-SHA1 Implementation
static void hmacSha1(const uint8_t* key, size_t keyLen, const uint8_t* msg, size_t msgLen, uint8_t output[20])
{
    uint8_t k[64] = {0};
    if (keyLen > 64)
    {
        SHA1 sha;
        sha.init();
        sha.update(key, keyLen);
        sha.final(k);
    }
    else
    {
        memcpy(k, key, keyLen);
    }

    uint8_t ipad[64];
    uint8_t opad[64];
    for (int i = 0; i < 64; i++)
    {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5C;
    }

    SHA1 shaInner;
    shaInner.init();
    shaInner.update(ipad, 64);
    shaInner.update(msg, msgLen);
    uint8_t innerDigest[20];
    shaInner.final(innerDigest);

    SHA1 shaOuter;
    shaOuter.init();
    shaOuter.update(opad, 64);
    shaOuter.update(innerDigest, 20);
    shaOuter.final(output);
}

// 🔑 TOTP Rolling Generator (6 digits)
static std::string generateTotp(const std::string& secret, uint64_t timeStep)
{
    auto key = base32Decode(secret);
    if (key.empty()) return "";

    uint8_t msg[8];
    for (int i = 7; i >= 0; i--)
    {
        msg[i] = static_cast<uint8_t>(timeStep & 0xFF);
        timeStep >>= 8;
    }

    uint8_t hash[20];
    hmacSha1(key.data(), key.size(), msg, 8, hash);

    int offset = hash[19] & 0xF;
    uint32_t binary = ((hash[offset] & 0x7F) << 24) |
                      ((hash[offset + 1] & 0xFF) << 16) |
                      ((hash[offset + 2] & 0xFF) << 8) |
                      (hash[offset + 3] & 0xFF);

    uint32_t otp = binary % 1000000;
    std::string result = std::to_string(otp);
    while (result.length() < 6)
    {
        result = "0" + result;
    }
    return result;
}

// 🔑 Base64 Encoder
static std::string base64Encode(const uint8_t* data, size_t len)
{
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result = "";
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--)
    {
        char_array_3[i++] = *(data++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                result += chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            result += chars[char_array_4[j]];

        while ((i++ < 3))
            result += '=';
    }

    return result;
}

// 🔑 Base64 Decoder
static std::vector<uint8_t> base64Decode(const std::string& input)
{
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<uint8_t> result;
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];

    while (in_ < input.size() && input[in_] != '=')
    {
        char_array_4[i++] = input[in_++];
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                result.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++)
            result.push_back(char_array_3[j]);
    }

    return result;
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

// 📂 Escape special characters for CSV format
static std::string escapeCsvString(const std::string& input)
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
static std::vector<std::vector<std::string>> parseCsvFile(const std::string& filePath)
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

// 🔍 Cryptographically secure random password generator
static std::string generatePassword(int length, bool upper, bool lower, bool digit, bool symbol)
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

        // 🔹 Bind: Add / Update Credential (archives old password to history)
        w.bind("api_add_credential", [&](std::string req) -> std::string {
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

            std::string json;
            if (site.empty() || user.empty() || pass.empty()) {
                json = "{\"success\":false,\"error\":\"All fields are required.\"}";
            } else {
                Credential credential(site, user, pass);
                credential.setTotpSecret(totpSecret);
                credential.setNotes(notes);
                credential.setAttachmentName(attachmentName);
                credential.setAttachmentData(attachmentData);

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