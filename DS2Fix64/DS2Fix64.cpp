#include "stdafx.h"
#include "DS2Fix64.h"
#include "Utils\Log.h"
#include "Utils\SigScan.h"
#include "Core\Signatures.h"
#include "Fixes\Durability.h"

#include "MinHook.h"

BOOL Begin()
{
    Log(">> hooked");

    Log(">> GetImageInfo()");
    if (!GetImageInfo())
    {
        Log(">>> GetImageInfo() failed");
        return false;
    }

    Log(">> FindSignature()");
    oApplyDurabilityDamage = (ApplyDurabilityDamage)(FindSignature(&fsApplyDurabilityDamage));
    if (oApplyDurabilityDamage != nullptr)
    {
        Log(">>> oApplyDurabilityDamage() @ 0x%p", oApplyDurabilityDamage);
    }
    else {
        Log(">>> failed to locate oApplyDurabilityDamage()");
        return false;
    }
    
    Log(">> ApplyDetours()");
    if (ApplyDetours() != MH_OK)
    {
        Log(">>> detouring failed");
        return false;
    }
    else {
        Log(">>> detours applied successfully");
    }
    Log(">>> tApplyDurabilityDamage() @ 0x%p", tApplyDurabilityDamage);
    Log(">>> bApplyDurabilityDamage() @ 0x%p", bApplyDurabilityDamage);


    return true;
}

BOOL End()
{
    Log(">> RemoveDetours()");
    if (RemoveDetours() != MH_OK)
    {
        Log(">>> failed to remove detours");
        return false;
    }
    else {
        Log(">>> detours removed successfully");
    }

    Log(">> unhooked");
    Log("> main thread exiting");

    return true;
}

int ApplyDetours()
{
    MH_STATUS hr;

    hr = MH_Initialize();
    if (hr != MH_OK)
    {
        Log(">>>> failed to initialize MinHook");
        return hr;
    }

    hr = MH_CreateHook((LPVOID*)oApplyDurabilityDamage, (LPVOID)tApplyDurabilityDamage, reinterpret_cast<LPVOID*>(&bApplyDurabilityDamage));
    if (hr != MH_OK)
    {
        Log(">>>> failed to hook oApplyDurabilityDamage");
        return hr;
    }

    hr = MH_EnableHook(oApplyDurabilityDamage);
    if (hr != MH_OK)
    {
        Log(">>>> failed to detour oApplyDurabilityDamage");
        return hr;
    }

    return hr;
}

int RemoveDetours()
{
    MH_STATUS hr;

    hr = MH_DisableHook(oApplyDurabilityDamage);
    if (hr != MH_OK)
    {
        Log(">>>> failed to remove detour for oApplyDurabilityDamage");
        return hr;
    }

    hr = MH_Uninitialize();
    if (hr != MH_OK)
    {
        Log(">>>> failed to uninitialize MinHook");
        return hr;
    }

    return hr;
}