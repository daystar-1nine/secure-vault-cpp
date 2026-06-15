#include "utils/SystemUtils.h"

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <objbase.h>
#endif

#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

// Native Win32 file dialogs
std::string openFileDialog(HWND hwnd)
{
#ifdef _WIN32
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
#endif
    return "";
}

std::string saveFileDialog(HWND hwnd)
{
#ifdef _WIN32
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
#endif
    return "";
}

// 🕒 Check system idle time (inactivity)
uint32_t getSystemIdleTimeMs()
{
#ifdef _WIN32
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lii))
    {
        DWORD now = GetTickCount();
        return now - lii.dwTime;
    }
#endif
    return 0;
}

// 📋 Conditional clipboard clearing
bool clearClipboardIfMatching(const std::string& expected)
{
#ifdef _WIN32
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
#endif
    return false;
}

// 📂 Register shortcut in the Windows Start Menu on startup
void createStartMenuShortcut()
{
#ifdef _WIN32
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
#endif
}
