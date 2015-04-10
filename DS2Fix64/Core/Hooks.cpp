#include "stdafx.h"
#include "Hooks.h"

#include "MinHook.h"

#include "Utils\SigScan.h"
#include "Core\Signatures.h"
#include "Fixes\Durability.h"
#include "Fixes\PlusFourteen.h"
#include "Matchmaking\Blocklist.h"
#include "Matchmaking\RegionFilter.h"
#include "Fixes\Assert.h"

ISteamFriends* sFriends = nullptr;

BOOL SteamHooks()
{
    // <Steam>
    //      get the handle for the Steam module and initialize MinHook
    HMODULE hSteam = nullptr;
    while (hSteam == nullptr)
    {
        hSteam = GetModuleHandle(TEXT("steam_api64.dll"));
        Sleep(500);
    }
    debug("hSteam @ 0x%p", hSteam);

    if (MH_Initialize() != MH_OK)
    {
        log_warn("failed to initialize MinHook for Steam (already initialized?)");
    }
    // </Steam>



    // <SteamFriends>
    //  handles: selective player blocking based on relationship status
    while (sFriends == nullptr)
    {
        sFriends = ((ISteamFriends*(*)(void))GetProcAddress(hSteam, "SteamFriends"))();
        Sleep(500);
    }
    debug("sFriends @ 0x%p", sFriends);
    // </SteamFriends>



    // <SteamNetworking>
    //  handles: accepting or dropping packets from players based on their relationship
    //      get the location for members SendP2PPacket() and ReadP2PPacket()
    //      from the ISteamNetworking VFT and enable their hooks
    ISteamNetworking* sNetwork = nullptr;
    while (sNetwork == nullptr)
    {
        sNetwork = ((ISteamNetworking*(*)(void))GetProcAddress(hSteam, "SteamNetworking"))();
        Sleep(500);
    }
    debug("sNetwork @ 0x%p", sNetwork);

    oSendP2PPacket = (SendP2PPacket)(*(uint64**)sNetwork)[0];
    oReadP2PPacket = (ReadP2PPacket)(*(uint64**)sNetwork)[2];

    if (MH_CreateHook((LPVOID*)oSendP2PPacket, (LPVOID)tSendP2PPacket, reinterpret_cast<LPVOID*>(&bSendP2PPacket)) != MH_OK ||
        MH_CreateHook((LPVOID*)oReadP2PPacket, (LPVOID)tReadP2PPacket, reinterpret_cast<LPVOID*>(&bReadP2PPacket)) != MH_OK)
    {
        log_err("failed to create hooks for SteamNetworking");
        return false;
    }

    if (MH_EnableHook(oSendP2PPacket) != MH_OK ||
        MH_EnableHook(oReadP2PPacket) != MH_OK)
    {
        log_err("failed to enable hooks for SteamNetworking");
        return false;
    }

    debug("oSendP2PPacket() @ 0x%p t-> 0x%p b-> 0x%p", oSendP2PPacket, tSendP2PPacket, bSendP2PPacket);
    debug("oReadP2PPacket() @ 0x%p t-> 0x%p b-> 0x%p", oReadP2PPacket, tReadP2PPacket, bReadP2PPacket);
    // </SteamNetworking>


    
    // <SteamMatchmaking>
    //  handles: selective region-based matchmaking
    //      get the location for member AddRequestLobbyListDistanceFilter()
    //      from the ISteamMatchmaking VFT and enable its hook
    ISteamMatchmaking* sMatchmaking = nullptr;
    while (sMatchmaking == nullptr)
    {
        sMatchmaking = ((ISteamMatchmaking*(*)(void))GetProcAddress(hSteam, "SteamMatchmaking"))();
        Sleep(500);
    }
    debug("sMatchmaking @ 0x%p", sMatchmaking);

    oCreateLobby = (CreateLobby)(*(uint64**)sMatchmaking)[13];
    oJoinLobby = (JoinLobby)(*(uint64**)sMatchmaking)[14];
    oLeaveLobby = (LeaveLobby)(*(uint64**)sMatchmaking)[15];

    if (MH_CreateHook((LPVOID*)oCreateLobby, (LPVOID)tCreateLobby, reinterpret_cast<LPVOID*>(&bCreateLobby)) != MH_OK ||
        MH_CreateHook((LPVOID*)oJoinLobby, (LPVOID)tJoinLobby, reinterpret_cast<LPVOID*>(&bJoinLobby)) != MH_OK ||
        MH_CreateHook((LPVOID*)oLeaveLobby, (LPVOID)tLeaveLobby, reinterpret_cast<LPVOID*>(&bLeaveLobby)) != MH_OK)
    {
        log_err("failed to create hooks for SteamMatchmaking");
        return false;
    }

    if (MH_EnableHook(oCreateLobby) != MH_OK ||
        MH_EnableHook(oJoinLobby) != MH_OK ||
        MH_EnableHook(oLeaveLobby) != MH_OK)
    {
        log_err("failed to enable hooks for SteamMatchmaking");
        return false;
    }

    debug("oCreateLobby() @ 0x%p t-> 0x%p b-> 0x%p", oCreateLobby, tCreateLobby, bCreateLobby);
    debug("oJoinLobby() @ 0x%p t-> 0x%p b-> 0x%p", oJoinLobby, tJoinLobby, bJoinLobby);
    debug("oLeaveLobby() @ 0x%p t-> 0x%p b-> 0x%p", oLeaveLobby, tLeaveLobby, bLeaveLobby);
    // </SteamMatchmaking>

    return true;
}

BOOL RemoveSteamHooks()
{
    if (MH_DisableHook(oSendP2PPacket) != MH_OK ||
        MH_DisableHook(oReadP2PPacket) != MH_OK ||
        MH_DisableHook(oCreateLobby) != MH_OK || 
        MH_DisableHook(oJoinLobby) != MH_OK ||
        MH_DisableHook(oLeaveLobby) != MH_OK)
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

    // #Namecrash / SEH assert.
    oNamecrashAssert = (NamecrashAssert)(FindSignature(&fsNamecrashAssert));
    PatchNamecrashAssert();

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
    debug("oNamecrashAssert @ 0x%p", oNamecrashAssert);

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