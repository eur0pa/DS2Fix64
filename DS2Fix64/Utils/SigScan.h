#pragma once
#include "stdafx.h"

struct Signature
{
    char* signature;
    char* mask;
    size_t length;
    DWORD ret;
};

BOOL GetImageInfo();
VOID* FindSignature(Signature* fnSig);
