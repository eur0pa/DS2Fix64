#include "stdafx.h"
#include "DS2Fix64.h"

#include "Core\Hooks.h"
#include "Core\Versions.h"

BOOL Begin()
{
    if (CheckDkSVersion() != 0)
    {
        log_err("CheckDkSVersion() failed: %Iu bytes", CheckDkSVersion());
        MessageBox(NULL,
            "This version of Dark Souls II: Scholar of the First Sin\n"
            "is not supported. DS2Fix64 will now unhook and the game\n"
            "will proceed as usual. Please update DS2Fix64.",
            "DS2Fix64 error", MB_OK);
        return false;
    }

    if (!GetImageInfo())
    {
        log_err("GetImageInfo() failed");
        return false;
    }

    // Apply game hooks
    if (GameHooks() == false)
    {
        log_err("Failed to apply game hooks");
        return false;
    }

    return true;
}

BOOL End()
{
    if (RemoveGameHooks() == false)
    {
        log_err("Failed to remove Game hooks");
        return false;
    }

    return true;
}


