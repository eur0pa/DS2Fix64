#include "stdafx.h"
#include "Hooks.h"

#include "MinHook.h"

#include "Utils\SigScan.h"
#include "Core\Signatures.h"
#include "Fixes\Durability.h"
#include "Fixes\PlusFourteen.h"
#include "Matchmaking\Matchmaking.h"


BOOL SteamHooks()
{
    HMODULE hSteam = nullptr;
    while (hSteam == nullptr)
    {
        hSteam = GetModuleHandle(TEXT("steam_api64.dll"));
        Sleep(1000);
    }
    debug("hSteam @ 0x%p", hSteam);

    ISteamNetworking* sNetwork = nullptr;
    while (sNetwork == nullptr)
    {
        sNetwork = ((ISteamNetworking*(*)(void))GetProcAddress(hSteam, "SteamNetworking"))();
        Sleep(1000);
    }
    debug("sNetwork @ 0x%p", sNetwork);

    oSendP2PPacket = (SendP2PPacket)(*(uint32**)sNetwork)[0];
    oReadP2PPacket = (ReadP2PPacket)(*(uint32**)sNetwork)[2];

    if (MH_Initialize() != MH_OK)
    {
        log_warn("failed to initialize MinHook for Steam (already initialized?)");
    }

    if (MH_CreateHook((LPVOID*)oSendP2PPacket, (LPVOID)tSendP2PPacket, reinterpret_cast<LPVOID*>(&bSendP2PPacket)) != MH_OK ||
        MH_CreateHook((LPVOID*)oReadP2PPacket, (LPVOID)tReadP2PPacket, reinterpret_cast<LPVOID*>(&bReadP2PPacket)) != MH_OK)
    {
        log_err("failed to create hooks for Steam");
        return false;
    }

    if (MH_EnableHook(oSendP2PPacket) != MH_OK ||
        MH_EnableHook(oReadP2PPacket) != MH_OK)
    {
        log_err("failed to enable hooks for Steam");
        return false;
    }

    debug("oSendP2PPacket() @ 0x%p t-> 0x%p b-> 0x%p", oSendP2PPacket, tSendP2PPacket, bSendP2PPacket);
    debug("oReadP2PPacket() @ 0x%p t-> 0x%p b-> 0x%p", oReadP2PPacket, tReadP2PPacket, bReadP2PPacket);

    return true;
}

BOOL RemoveSteamHooks()
{
    if (MH_DisableHook(oSendP2PPacket) != MH_OK ||
        MH_DisableHook(oReadP2PPacket) != MH_OK)
    {
        return false;
    }

    if (MH_Uninitialize() != MH_OK)
    {
        log_warn("Failed to uninitialize MinHook for Steam (already uninitialized?)");
    }

    return true;
}

BOOL GameHooks()
{
    // Retrieve injections points by signature

    // This one is a complete hook, we'll return to the trampoline
    // and back to the original
    oApplyDurabilityDamage = (ApplyDurabilityDamage)(FindSignature(&fsApplyDurabilityDamage));

    // +14 patching is mid-function hooking, so we have to manage
    // the return address differently. The first hook returns 8 bytes
    // below the injection point.
    oPlusFourteen_1 = (PlusFourteen_1)(FindSignature(&fsPlusFourteenCrash_1));
    bPlusFourteen_1 = (DWORD64)oPlusFourteen_1 + fsPlusFourteenCrash_1.ret;

    // The second hook needs 11 bytes. Got a better way? Create a pull request!
    oPlusFourteen_2 = (PlusFourteen_2)(FindSignature(&fsPlusFourteenCrash_2));
    bPlusFourteen_2 = (DWORD64)oPlusFourteen_2 + fsPlusFourteenCrash_2.ret;

    if (MH_Initialize() != MH_OK)
    {
        log_err("Failed to initialize MinHook");
        return false;
    }

    if (MH_CreateHook((LPVOID*)oApplyDurabilityDamage, (LPVOID)tApplyDurabilityDamage, reinterpret_cast<LPVOID*>(&bApplyDurabilityDamage)) != MH_OK ||
        MH_CreateHook((LPVOID*)oPlusFourteen_1, (LPVOID)tPlusFourteen_1, NULL) != MH_OK ||
        MH_CreateHook((LPVOID*)oPlusFourteen_2, (LPVOID)tPlusFourteen_2, NULL) != MH_OK)
    {
        log_err("Failed to create hooks");
        return false;
    }

    if (MH_EnableHook(oApplyDurabilityDamage) != MH_OK ||
        MH_EnableHook(oPlusFourteen_1) != MH_OK ||
        MH_EnableHook(oPlusFourteen_2) != MH_OK)
    {
        return false;
    }

    debug("oApplyDurabilityDamage() @ 0x%p t-> 0x%p b-> 0x%p", oApplyDurabilityDamage, tApplyDurabilityDamage, bApplyDurabilityDamage);
    debug("oPlusFourteen_1 @ 0x%p t-> 0x%p b-> 0x%p", oPlusFourteen_1, tPlusFourteen_1, bPlusFourteen_1);
    debug("oPlusFourteen_2 @ 0x%p t-> 0x%p b-> 0x%p", oPlusFourteen_2, tPlusFourteen_2, bPlusFourteen_2);

    return true;
}

BOOL RemoveGameHooks()
{
    if (MH_DisableHook(oApplyDurabilityDamage) != MH_OK ||
        MH_DisableHook(oPlusFourteen_1) != MH_OK ||
        MH_DisableHook(oPlusFourteen_2) != MH_OK)
    {
        return false;
    }

    if (MH_Uninitialize() != MH_OK)
    {
        log_err("failed to uninitialize MinHook");
        return false;
    }

    return true;
}