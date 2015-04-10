#include "stdafx.h"
#include "Assert.h"

NamecrashAssert oNamecrashAssert = nullptr;

// Ported from Marisa's fix
// @ https://github.com/OrdinaryMagician/Dks2Util/blob/master/src/InjectUtil.c

BYTE tNamecrashAssert[] = {
    0xEB, 0x08,             // jmp short +08
    0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90,
    0x90
};

BOOL PatchNamecrashAssert()
{
    DWORD oldProtect;
    VirtualProtect((LPVOID)oNamecrashAssert, sizeof(tNamecrashAssert), PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(oNamecrashAssert, tNamecrashAssert, sizeof(tNamecrashAssert));
    VirtualProtect((LPVOID)oNamecrashAssert, sizeof(tNamecrashAssert), oldProtect, &oldProtect);
    return true;
}