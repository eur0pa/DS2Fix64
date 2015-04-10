#include "stdafx.h"
#include "Blocklist.h"

SendP2PPacket oSendP2PPacket = nullptr;
SendP2PPacket bSendP2PPacket = nullptr;

ReadP2PPacket oReadP2PPacket = nullptr;
ReadP2PPacket bReadP2PPacket = nullptr;

EFriendRelationship kFRsend;
EFriendRelationship kFRrecv;

extern ISteamFriends* sFriends;

bool bReadRes;
bool bFriendsOnly = true;

bool __fastcall tSendP2PPacket(void* __this, CSteamID steamIDRemote, const void* pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel)
{
    kFRsend = sFriends->GetFriendRelationship(steamIDRemote);

    if (bFriendsOnly)
    {
        if (kFRsend != k_EFriendRelationshipFriend)
        {
            debug("SendP2PPacket() -> %I64d (kFR = %d) [discarded: friends only]", steamIDRemote.ConvertToUint64(), kFRsend);
            return false;
        }
    }
    else {
        if (kFRsend == k_EFriendRelationshipBlocked ||
            kFRsend == k_EFriendRelationshipIgnored ||
            kFRsend == k_EFriendRelationshipIgnoredFriend)
        {
            debug("SendP2PPacket() -> %I64d (kFR = %d) [discarded: blocklist]", steamIDRemote.ConvertToUint64(), kFRsend);
            return false;
        }
    }

    return bSendP2PPacket(__this, steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
}

bool __fastcall tReadP2PPacket(void* __this, void* pubDest, uint32 cubDest, uint32* pcubMsgSize, CSteamID* psteamIDRemote, int nChannel)
{
    bReadRes = bReadP2PPacket(__this, pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    kFRrecv = sFriends->GetFriendRelationship(*psteamIDRemote);

    if (bFriendsOnly)
    {
        if (kFRrecv != k_EFriendRelationshipFriend)
        {
            debug("ReadP2PPacket() <- %I64d (kFR = %d) [discarded: friends only]", psteamIDRemote->ConvertToUint64(), kFRrecv);
            return false;
        }
    }
    else {
        if (kFRrecv == k_EFriendRelationshipBlocked ||
            kFRrecv == k_EFriendRelationshipIgnored ||
            kFRrecv == k_EFriendRelationshipIgnoredFriend)
        {
            debug("ReadP2PPacket() <- %I64d (kFR = %d) [discarded: blocklist]", psteamIDRemote->ConvertToUint64(), kFRrecv);
            return false;
        }
    }

    return bReadRes;
}