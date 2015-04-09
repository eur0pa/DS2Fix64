#pragma once
#include "stdafx.h"
#include "Libs\steam\steam_api.h"

typedef bool(__stdcall *SendP2PPacket)(CSteamID, const void*, uint32, EP2PSend, int);
typedef bool(__stdcall *ReadP2PPacket)(void*, uint32, uint32*, CSteamID*, int);

extern SendP2PPacket oSendP2PPacket;
extern SendP2PPacket bSendP2PPacket;

extern ReadP2PPacket oReadP2PPacket;
extern ReadP2PPacket bReadP2PPacket;

extern bool tSendP2PPacket(CSteamID, const void*, uint32, EP2PSend, int);
extern bool tReadP2PPacket(void*, uint32, uint32*, CSteamID*, int);