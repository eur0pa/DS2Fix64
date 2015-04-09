#include "stdafx.h"
#include "Versions.h"

struct
{
    TCHAR* sVersion;
    SIZE_T szSize;
} SotFSVersions[]
{
    { "1.0.1", 31663976 }
};

SIZE_T CheckDkSVersion()
{
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    FILE *p_file = nullptr;
    p_file = fopen(buffer, "rb");
    fseek(p_file, 0, SEEK_END);
    SIZE_T szSize = ftell(p_file);
    fclose(p_file);

    for (auto x = 0; x < _countof(SotFSVersions); x++)
    {
        if (szSize == SotFSVersions[x].szSize)
        {
            szSize = 0;
        }
    }

    return szSize;
}