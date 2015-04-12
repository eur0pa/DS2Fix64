#pragma once
#include "stdafx.h"
#include "Libs\steam\steam_api.h"

typedef SteamAPICall_t(__thiscall* RequestLobbyList)(void*);                // vtf[4]
typedef SteamAPICall_t(__thiscall* CreateLobby)(void*, ELobbyType, int);    // vft[13]
typedef SteamAPICall_t(__thiscall* JoinLobby)(void*, CSteamID);             // vft[14]
typedef SteamAPICall_t(__thiscall* LeaveLobby)(void*, CSteamID);            // vft[15]
typedef int(__thiscall* GetNumLobbyMembers)(void*, CSteamID);               // vft[17]

extern RequestLobbyList oRequestLobbyList, bRequestLobbyList;
extern CreateLobby oCreateLobby, bCreateLobby;
extern JoinLobby oJoinLobby, bJoinLobby;
extern LeaveLobby oLeaveLobby, bLeaveLobby;
extern GetNumLobbyMembers oGetNumLobbyMembers, bGetNumLobbyMembers;

extern SteamAPICall_t __fastcall tRequestLobbyList(void*);
extern SteamAPICall_t __fastcall tCreateLobby(void*, ELobbyType, int);
extern SteamAPICall_t __fastcall tJoinLobby(void*, CSteamID);
extern SteamAPICall_t __fastcall tLeaveLobby(void*, CSteamID);
extern int __fastcall tGetNumLobbyMembers(void*, CSteamID);
