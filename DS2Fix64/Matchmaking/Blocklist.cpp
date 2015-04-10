#include "stdafx.h"
#include "Blocklist.h"

SendP2PPacket oSendP2PPacket = nullptr;
SendP2PPacket bSendP2PPacket = nullptr;
ReadP2PPacket oReadP2PPacket = nullptr;
ReadP2PPacket bReadP2PPacket = nullptr;
ISteamFriends* sFriends = nullptr;

EFriendRelationship kFRsend;
EFriendRelationship kFRrecv;

bool bReadRes;

bool __fastcall tSendP2PPacket(void* __this, CSteamID steamIDRemote, const void* pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel)
{
    kFRsend = sFriends->GetFriendRelationship(steamIDRemote);

    if (kFRsend == k_EFriendRelationshipBlocked ||
        kFRsend == k_EFriendRelationshipIgnored ||
        kFRsend == k_EFriendRelationshipIgnoredFriend)
    {
        debug("SendP2PPacket() -> %u (kFR = %d) [discarded]", steamIDRemote.ConvertToUint64(), kFRsend);
        return false;
    }

    debug("SendP2PPacket() -> %u (kFR = %d) [accepted]", steamIDRemote.ConvertToUint64(), kFRsend);
    return bSendP2PPacket(__this, steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
}

bool __fastcall tReadP2PPacket(void* __this, void* pubDest, uint32 cubDest, uint32* pcubMsgSize, CSteamID* psteamIDRemote, int nChannel)
{
    bReadRes = bReadP2PPacket(__this, pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    kFRrecv = sFriends->GetFriendRelationship(*psteamIDRemote);

    if (kFRrecv == k_EFriendRelationshipBlocked ||
        kFRrecv == k_EFriendRelationshipIgnored ||
        kFRrecv == k_EFriendRelationshipIgnoredFriend)
    {
        debug("ReadP2PPacket() <- %u (kFR = %d) [discarded]", psteamIDRemote->ConvertToUint64(), kFRrecv);
        return false;
    }

    debug("ReadP2PPacket() <- %u (kFR = %d) [accepted]", psteamIDRemote->ConvertToUint64(), kFRrecv);
    return bReadRes;
}