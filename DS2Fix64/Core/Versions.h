#pragma once
#include "stdafx.h"

extern SIZE_T CheckDkSVersion();

struct
{
    TCHAR* sVersion;
    SIZE_T szSize;
} SotFSVersions[] {
    { "1.0.1", 31663976 }
};