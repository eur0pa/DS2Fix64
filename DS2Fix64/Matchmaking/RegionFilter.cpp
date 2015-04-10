#include "stdafx.h"
#include "RegionFilter.h"

CreateLobby oCreateLobby = nullptr;
CreateLobby bCreateLobby = nullptr;

JoinLobby oJoinLobby = nullptr;
JoinLobby bJoinLobby = nullptr;

LeaveLobby oLeaveLobby = nullptr;
LeaveLobby bLeaveLobby = nullptr;

ISteamMatchmaking* sMatchmaking = nullptr;

void __fastcall tCreateLobby(void* __this, ELobbyType eLobbyType, int cMaxMembers)
{
    debug("CreateLobby(eLobbyType = %u, cMaxMembers = %d)", eLobbyType, cMaxMembers);
    return bCreateLobby(__this, eLobbyType, cMaxMembers);
}

void __fastcall tJoinLobby(void* __this, CSteamID steamIDLobby)
{
    debug("JoinLobby(CSteamID = %u)", steamIDLobby.ConvertToUint64());
    return bJoinLobby(__this, steamIDLobby);
}

void __fastcall tLeaveLobby(void* __this, CSteamID steamIDLobby)
{
    debug("LeaveLobby(CSteamID = %u)", steamIDLobby.ConvertToUint64());
    return bLeaveLobby(__this, steamIDLobby);
}