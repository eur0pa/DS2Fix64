#include "stdafx.h"
#include "Matchmaking.h"

pSendP2PPacket oSendP2PPacket = nullptr;
pReadP2PPacket oReadP2PPacket = nullptr;

bool tSendP2PPacket(CSteamID steamIDRemote, const void* pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel)
{
    debug("tSendP2PPacket(steamIDRemote = %llu, pubData, cubData = %u, eP2PSendType = %d, nChannel = %d)",
        steamIDRemote.ConvertToUint64, cubData, eP2PSendType, nChannel);
    return oSendP2PPacket(steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
}

bool tReadP2PPacket(void* pubDest, uint32 cubDest, uint32* pcubMsgSize, CSteamID* psteamIDRemote, int nChannel)
{
    debug("tReadP2PPacket(pubDest, cubDest = %u, pcubMsgSize = %u, psteamIDRemote = %llu, nChannel = %d)",
        cubDest, pcubMsgSize, psteamIDRemote->ConvertToUint64(), nChannel);
    return oReadP2PPacket(pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
}