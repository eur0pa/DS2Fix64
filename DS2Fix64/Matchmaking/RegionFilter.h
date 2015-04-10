#pragma once
#include "stdafx.h"
#include "Libs\steam\steam_api.h"

typedef void(__thiscall* CreateLobby)(void*, ELobbyType, int);
typedef void(__thiscall* JoinLobby)(void*, CSteamID);
typedef void(__thiscall* LeaveLobby)(void*, CSteamID);

extern CreateLobby oCreateLobby;
extern CreateLobby bCreateLobby;

extern JoinLobby oJoinLobby;
extern JoinLobby bJoinLobby;

extern LeaveLobby oLeaveLobby;
extern LeaveLobby bLeaveLobby;

extern void __fastcall tCreateLobby(void*, ELobbyType, int);
extern void __fastcall tJoinLobby(void*, CSteamID);
extern void __fastcall tLeaveLobby(void*, CSteamID);