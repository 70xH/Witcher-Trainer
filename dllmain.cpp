// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Include.h"

DWORD JumpBack;
DWORD pHero;

// Shell code to get the player address
void __declspec(naked) WhiteWolf()
{
    _asm {
        push eax
        mov eax, [ecx + 0x0000007c]
        mov[pHero], ecx
        pop eax
        fld dword ptr[ecx + 54]
        fadd dword ptr[ecx + 48]
        jmp[JumpBack]
    }
}

DWORD WINAPI HackThread(LPVOID hModule)
{
    // Get access to console for additional information
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    if (file == NULL)
    {
        return -1;
    }

    printf("Witcher 1 Enhanced Edition Trainer\n");

    // Witcher module base
    DWORD withcerBase = (DWORD)GetModuleHandle(L"witcher.exe");
    
    // Hooking to gain player address
    int hookLength = 6;
    int targetLoc = withcerBase + 0x37A59D;

    // Jump back address
    JumpBack = targetLoc + hookLength;

    Slash((void*)targetLoc, WhiteWolf, hookLength);

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        if (pHero)
        {
            printf("pHero: %X\n", pHero);
        }

        Sleep(50);
    }

    // Retrive back to the original bytes
    BYTE bytes[] = { 0xd9, 0x41, 0x54, 0xd8, 0x41, 0x48 };
    BackHome((void*)targetLoc, bytes, hookLength);

    if (file != NULL) fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, HackThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}