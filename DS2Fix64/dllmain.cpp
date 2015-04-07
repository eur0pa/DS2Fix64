#include "stdafx.h"

#include "dinput8\dinputWrapper.h"
#include "DS2Fix64.h"

// Export DINPUT8
tDirectInput8Create oDirectInput8Create;

BOOL InitInstance(HMODULE hModule)
{
    log_info("Dark Souls II Fix loaded");

    // Load the real dinput8.dll
    HMODULE hMod;
    char dllPath[MAX_PATH];
    GetSystemDirectory(dllPath, MAX_PATH);
    strcat_s(dllPath, "\\dinput8.dll");
    hMod = LoadLibrary(dllPath);
    oDirectInput8Create = (tDirectInput8Create)GetProcAddress(hMod, "DirectInput8Create");
    
    // Call the main thread
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, hModule, 0, nullptr);

    return true;
}

BOOL ExitInstance()
{
    End();
    log_info("Dark Souls II Fix unloaded");

    return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        InitInstance(hModule);
        break;
    case DLL_PROCESS_DETACH:
        ExitInstance();
        break;
    }
    return TRUE;
}

