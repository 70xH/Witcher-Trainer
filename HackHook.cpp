#include "pch.h"
#include "Include.h"

bool Slash(void* targetLoc, void* controlPoint, int hookLength)
{
    if (hookLength < 5)
    {
        return false;
    }

    // Change the memory protection to read, write and executable
    DWORD prevProtection;
    VirtualProtect(targetLoc, hookLength, PAGE_EXECUTE_READWRITE, &prevProtection);

    // Set all the bytes to NOP
    memset(targetLoc, 0x90, hookLength);

    // Get relative address of control point and target location
    DWORD relativeAddress = ((DWORD)controlPoint - (DWORD)targetLoc) - 5;

    // Set the first byte to 0xE9, indicating jump instruction
    *(BYTE*)targetLoc = 0xE9;
    *(DWORD*)((DWORD)targetLoc + 1) = relativeAddress;

    // Set back the protection
    VirtualProtect(targetLoc, hookLength, prevProtection, &prevProtection);

    return true;
}

bool BackHome(void* targetLoc, BYTE bytes[], int hookLength)
{
    if (hookLength < 5)
    {
        return false;
    }

    // Change the memory protection to read, write and executable
    DWORD prevProtection;
    VirtualProtect(targetLoc, hookLength, PAGE_EXECUTE_READWRITE, &prevProtection);

    // Copy the original bytes
    memcpy(targetLoc, (void*)bytes, hookLength);

    // Set back the protection
    VirtualProtect(targetLoc, hookLength, prevProtection, &prevProtection);

    return true;
}