#include "utils/SecurityUtils.h"

#ifdef _WIN32
#include <windows.h>
#endif

void secureWipeString(std::string& s)
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

void secureLockString(std::string& s)
{
    if (s.empty()) return;
#ifdef _WIN32
    VirtualLock(const_cast<char*>(s.data()), s.size());
#endif
}
