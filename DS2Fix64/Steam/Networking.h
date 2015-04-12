#pragma once
#include "stdafx.h"
#include "Libs\steam\steam_api.h"

typedef bool(__thiscall* SendP2PPacket)(void*, CSteamID, const void*, uint32, EP2PSend, int);
typedef bool(__thiscall* ReadP2PPacket)(void*, void*, uint32, uint32*, CSteamID*, int);

extern SendP2PPacket oSendP2PPacket, bSendP2PPacket;
extern ReadP2PPacket oReadP2PPacket, bReadP2PPacket;

extern bool __fastcall tSendP2PPacket(void*, CSteamID, const void*, uint32, EP2PSend, int);
extern bool __fastcall tReadP2PPacket(void*, void*, uint32, uint32*, CSteamID*, int);

extern ISteamFriends* sFriends;