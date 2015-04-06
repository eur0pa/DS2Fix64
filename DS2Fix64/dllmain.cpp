#include "stdafx.h"
#include "dinput8\dinputWrapper.h"
#include "Utils\Log.h"
#include "DS2Fix64.h"

// Export DINPUT8
tDirectInput8Create oDirectInput8Create;

const char* logFilePath = nullptr;
HANDLE hFile;

BOOL InitInstance(HMODULE hModule)
{
    // Init logging facility
    logFilePath = "DS2fix.log";
    hFile = CreateFile(logFilePath, FILE_GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    Log("Dark Souls II Fix loaded");

    // Load the real dinput8.dll
    HMODULE hMod;
    char dllPath[MAX_PATH];
    GetSystemDirectory(dllPath, MAX_PATH);
    strcat_s(dllPath, "\\dinput8.dll");
    hMod = LoadLibrary(dllPath);
    oDirectInput8Create = (tDirectInput8Create)GetProcAddress(hMod, "DirectInput8Create");
    Log("> dinput8.dll wrapped and loaded");
    
    // Call the main thread
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, hModule, 0, nullptr);
    Log("> main thread created");

    return true;
}

BOOL ExitInstance()
{
    End();
    Log("Dark Souls II Fix unloaded");

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

