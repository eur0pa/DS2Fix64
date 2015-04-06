#include "stdafx.h"
#include "SigScan.h"

VOID* pBaseAddress = nullptr;
size_t szImageLength = 0;

BOOL GetImageInfo()
{
    BOOL bSuccess = false;
    HMODULE hMod = GetModuleHandle(NULL);

    if (hMod)
    {
        MEMORY_BASIC_INFORMATION memInfo;

        if (VirtualQuery((void*)hMod, &memInfo, sizeof(memInfo)) != 0)
        {
            IMAGE_DOS_HEADER* hDos = (IMAGE_DOS_HEADER*)hMod;
            IMAGE_NT_HEADERS* hPe = (IMAGE_NT_HEADERS*)((ULONG64)memInfo.AllocationBase + (ULONG64)hDos->e_lfanew);

            if ((hDos->e_magic == IMAGE_DOS_SIGNATURE) && (hPe->Signature == IMAGE_NT_SIGNATURE))
            {
                bSuccess = true;
                pBaseAddress = (void*)memInfo.AllocationBase;
                szImageLength = (SIZE_T)hPe->OptionalHeader.SizeOfImage;
             
            }
        }
    }
    return bSuccess;
}

VOID* FindSignature(Signature* fnSig)
{
    TCHAR* pScan = (TCHAR*)pBaseAddress;
    INT iMaxLength = 0;

    while (pScan < ((TCHAR*)pBaseAddress + szImageLength - fnSig->length))
    {
        SIZE_T szLength = 0;

        for (INT i = 0; i < fnSig->length; i++)
        {
            if (!((pScan[i] == fnSig->signature[i]) || (fnSig->mask[i] == '?')))
            {
                break;
            }
            szLength++;
        }

        if (szLength > iMaxLength)
        {
            iMaxLength = (INT)szLength;
        }

        if (szLength == fnSig->length)
        {
            return pScan;
        }

        pScan++;
    }

    return nullptr;
}