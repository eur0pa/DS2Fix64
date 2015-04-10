#include "stdafx.h"
#include "RegionFilter.h"

CreateLobby oCreateLobby = nullptr;
CreateLobby bCreateLobby = nullptr;

JoinLobby oJoinLobby = nullptr;
JoinLobby bJoinLobby = nullptr;

LeaveLobby oLeaveLobby = nullptr;
LeaveLobby bLeaveLobby = nullptr;

void __fastcall tCreateLobby(void* __this, ELobbyType eLobbyType, int cMaxMembers)
{
    debug("CreateLobby(eLobbyType = %I64d, cMaxMembers = %d)", eLobbyType, cMaxMembers);
    return bCreateLobby(__this, eLobbyType, cMaxMembers);
}

void __fastcall tJoinLobby(void* __this, CSteamID steamIDLobby)
{
    debug("JoinLobby(CSteamID = %I64d)", steamIDLobby.ConvertToUint64());
    return bJoinLobby(__this, steamIDLobby);
}

void __fastcall tLeaveLobby(void* __this, CSteamID steamIDLobby)
{
    debug("LeaveLobby(CSteamID = %I64d)", steamIDLobby.ConvertToUint64());
    return bLeaveLobby(__this, steamIDLobby);
}