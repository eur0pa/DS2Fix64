#pragma once
#include "stdafx.h"
#include "Libs\steam\steam_api.h"

typedef bool(__stdcall *pSendP2PPacket)(CSteamID, const void*, uint32, EP2PSend, int);
typedef bool(__stdcall *pReadP2PPacket)(void*, uint32, uint32*, CSteamID*, int);

extern pSendP2PPacket oSendP2PPacket;
extern pReadP2PPacket oReadP2PPacket;