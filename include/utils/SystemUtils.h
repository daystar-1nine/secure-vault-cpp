#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <string>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
typedef void* HWND;
#endif

std::string openFileDialog(HWND hwnd);
std::string saveFileDialog(HWND hwnd);
uint32_t getSystemIdleTimeMs();
bool clearClipboardIfMatching(const std::string& expected);
void createStartMenuShortcut();

#endif
