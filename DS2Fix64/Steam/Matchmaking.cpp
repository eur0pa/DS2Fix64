#include "stdafx.h"
#include "Matchmaking.h"

RequestLobbyList 
    oRequestLobbyList = nullptr,
    bRequestLobbyList = nullptr;

CreateLobby
    oCreateLobby = nullptr,
    bCreateLobby = nullptr;

JoinLobby
    oJoinLobby = nullptr,
    bJoinLobby = nullptr;

LeaveLobby
    oLeaveLobby = nullptr,
    bLeaveLobby = nullptr;

GetNumLobbyMembers
    oGetNumLobbyMembers = nullptr,
    bGetNumLobbyMembers = nullptr;

SteamAPICall_t __fastcall tRequestLobbyList(void* __this)
{
    debug("RequestLobbyList()");
    return bRequestLobbyList(__this);
}

SteamAPICall_t __fastcall tCreateLobby(void* __this, ELobbyType eLobbyType, int cMaxMembers)
{
    debug("CreateLobby(eLobbyType = %I64d, cMaxMembers = %d)", eLobbyType, cMaxMembers);
    return bCreateLobby(__this, eLobbyType, cMaxMembers);
}

SteamAPICall_t __fastcall tJoinLobby(void* __this, CSteamID steamIDLobby)
{
    debug("JoinLobby(steamIDLobby = %I64d)", steamIDLobby.ConvertToUint64());
    return bJoinLobby(__this, steamIDLobby);
}

SteamAPICall_t __fastcall tLeaveLobby(void* __this, CSteamID steamIDLobby)
{
    debug("LeaveLobby(steamIDLobby = %I64d)", steamIDLobby.ConvertToUint64());
    return bLeaveLobby(__this, steamIDLobby);
}

int __fastcall tGetNumLobbyMembers(void* __this, CSteamID steamIDLobby)
{
    debug("GetNumLobbyMembers(steamIDLobby = %I64d)", steamIDLobby.ConvertToUint64());
    return bGetNumLobbyMembers(__this, steamIDLobby);
}