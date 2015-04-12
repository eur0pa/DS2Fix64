#include "stdafx.h"
#include "Matchmaking.h"

RequestLobbyList oRequestLobbyList = nullptr,
                 bRequestLobbyList = nullptr;

CreateLobby oCreateLobby = nullptr,
            bCreateLobby = nullptr;

JoinLobby oJoinLobby = nullptr,
          bJoinLobby = nullptr;

LeaveLobby oLeaveLobby = nullptr,
           bLeaveLobby = nullptr;

GetNumLobbyMembers oGetNumLobbyMembers = nullptr,
                   bGetNumLobbyMembers = nullptr;

GetLobbyMemberByIndex oGetLobbyMemberByIndex = nullptr,
                      bGetLobbyMemberByIndex = nullptr;

GetLobbyOwner oGetLobbyOwner = nullptr,
              bGetLobbyOwner = nullptr;

SteamAPICall_t __fastcall tRequestLobbyList(void* __this)
{
    SteamAPICall_t hSteamAPICall = bRequestLobbyList(__this);
    debug("RequestLobbyList() -> %I64d", hSteamAPICall);
    return hSteamAPICall;
}

SteamAPICall_t __fastcall tCreateLobby(void* __this, ELobbyType eLobbyType, int cMaxMembers)
{
    SteamAPICall_t hSteamAPICall = bCreateLobby(__this, eLobbyType, cMaxMembers);
    debug("CreateLobby(eLobbyType = %I64d, cMaxMembers = %d) -> %I64d", eLobbyType, cMaxMembers, hSteamAPICall);
    return hSteamAPICall;
}

SteamAPICall_t __fastcall tJoinLobby(void* __this, CSteamID steamIDLobby)
{
    SteamAPICall_t hSteamAPICall = bJoinLobby(__this, steamIDLobby);
    debug("JoinLobby(steamIDLobby = %I64d) -> %I64d", steamIDLobby.ConvertToUint64(), hSteamAPICall);
    return hSteamAPICall;
}

SteamAPICall_t __fastcall tLeaveLobby(void* __this, CSteamID steamIDLobby)
{
    SteamAPICall_t hSteamAPICall = bLeaveLobby(__this, steamIDLobby);
    debug("LeaveLobby(steamIDLobby = %I64d) -> %I64d", steamIDLobby.ConvertToUint64(), hSteamAPICall);
    return hSteamAPICall;
}

int __fastcall tGetNumLobbyMembers(void* __this, CSteamID steamIDLobby)
{
    int hSteamAPICall = bGetNumLobbyMembers(__this, steamIDLobby);
    debug("GetNumLobbyMembers(steamIDLobby = %I64d) -> %d", steamIDLobby.ConvertToUint64(), hSteamAPICall);
    return hSteamAPICall;
}

CSteamID __fastcall tGetLobbyMemberByIndex(void* __this, CSteamID steamIDLobby, int iMember)
{
    CSteamID hSteamAPICall = bGetLobbyMemberByIndex(__this, steamIDLobby, iMember);
    debug("GetNumLobbyMembers(steamIDLobby = %I64d, iMember = %d) -> %I64d", steamIDLobby.ConvertToUint64(), iMember, hSteamAPICall.ConvertToUint64());
    return hSteamAPICall;
}

CSteamID __fastcall tGetLobbyOwner(void* __this, CSteamID steamIDLobby)
{
    CSteamID hSteamAPICall = bGetLobbyOwner(__this, steamIDLobby);
    debug("tGetLobbyOwner(steamIDLobby = %I64d) -> %I64d", steamIDLobby.ConvertToUint64(), hSteamAPICall.ConvertToUint64());
    return hSteamAPICall;
}