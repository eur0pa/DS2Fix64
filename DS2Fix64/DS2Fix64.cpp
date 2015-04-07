#include "stdafx.h"
#include "DS2Fix64.h"
#include "Utils\SigScan.h"
#include "Core\Signatures.h"
#include "Fixes\Durability.h"

#include "MinHook.h"

BOOL Begin()
{
    if (!GetImageInfo())
    {
        log_err(">>> GetImageInfo() failed");
        return false;
    }

    oApplyDurabilityDamage = (ApplyDurabilityDamage)(FindSignature(&fsApplyDurabilityDamage));
    if (oApplyDurabilityDamage != nullptr)
    {
        debug(">>> oApplyDurabilityDamage() @ 0x%p", oApplyDurabilityDamage);
    }
    else {
        log_err(">>> failed to locate oApplyDurabilityDamage()");
        return false;
    }
    
    if (ApplyDetours() != MH_OK)
    {
        log_err(">>> detouring failed");
        return false;
    }

    debug(">>> tApplyDurabilityDamage() @ 0x%p", tApplyDurabilityDamage);
    debug(">>> bApplyDurabilityDamage() @ 0x%p", bApplyDurabilityDamage);

    return true;
}

BOOL End()
{
    if (RemoveDetours() != MH_OK)
    {
        log_err(">>> failed to remove detours");
        return false;
    }

    return true;
}

int ApplyDetours()
{
    MH_STATUS hr;

    hr = MH_Initialize();
    if (hr != MH_OK)
    {
        log_err(">>>> failed to initialize MinHook");
        return hr;
    }

    hr = MH_CreateHook((LPVOID*)oApplyDurabilityDamage, (LPVOID)tApplyDurabilityDamage, reinterpret_cast<LPVOID*>(&bApplyDurabilityDamage));
    if (hr != MH_OK)
    {
        log_err(">>>> failed to create hook");
        return hr;
    }

    hr = MH_EnableHook(oApplyDurabilityDamage);
    if (hr != MH_OK)
    {
        log_err(">>>> failed to enable hook");
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
        log_err(">>>> failed to remove hooks");
        return hr;
    }

    hr = MH_Uninitialize();
    if (hr != MH_OK)
    {
        log_err(">>>> failed to uninitialize MinHook");
        return hr;
    }

    return hr;
}