#include "stdafx.h"
#include "Log.h"

using std::string;

extern const char* logFile;
extern HANDLE hFile;

void Log(std::string format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    format += "\r\n";
    vsnprintf_s(buffer, sizeof(buffer), format.c_str(), args);
    va_end(args);
    if (!IsDebuggerPresent()) {
        WriteFile(hFile, buffer, (DWORD)strlen(buffer), NULL, NULL);
    }
    else {
        OutputDebugString(buffer);
    }
}