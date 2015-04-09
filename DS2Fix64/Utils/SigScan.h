#pragma once
#include "stdafx.h"

struct Signature
{
    char* signature;
    char* mask;
    size_t length;
    DWORD ret;
};

extern BOOL GetImageInfo();
extern VOID* FindSignature(Signature* fnSig);
