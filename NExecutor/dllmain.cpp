#include "pch.h"
#include "ExecutorConsole.h"

static void* scanForPointer(void* toFind)
{
    auto process = GetCurrentProcess();

    DWORD stackLow = 0, stackHigh = 0;
    GetCurrentThreadStackLimits(&stackLow, &stackHigh);

    MEMORY_BASIC_INFORMATION memoryInfo;
    for (DWORD regionAddress = 0;
        VirtualQueryEx(process, (void*)regionAddress, &memoryInfo, sizeof(memoryInfo));
        regionAddress += memoryInfo.RegionSize)
    {
        if ((regionAddress < stackLow || regionAddress > stackHigh) &&
            memoryInfo.State == MEM_COMMIT && memoryInfo.Type == MEM_PRIVATE && memoryInfo.Protect == PAGE_READWRITE)
        {
            for (DWORD i = 0; memoryInfo.RegionSize - i > sizeof(void*); ++i)
            {
                auto pointer = (void**)(regionAddress + i);

                if (pointer && *pointer == toFind)
                    return pointer;
            }
        }
    }

    return nullptr;
}

static DWORD __stdcall onDllAttached(LPVOID)
{
    ExecutorConsole executorConsole;
    if (!executorConsole.CreateConsole())
    {
        MessageBox(nullptr, "Could not prepare a console window!", "NExecutor", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    // Find ScriptContext, without having to make any hooks
    auto scriptContextPointer = scanForPointer(scriptContextVftable);
    if (!scriptContextPointer)
    {
        executorConsole.Write(ExecutorConsole::Error, "Could not find ScriptContext!");
        return EXIT_FAILURE;
    }

#ifdef _DEBUG
    executorConsole.WriteFormat(ExecutorConsole::Output, "ScriptContext: 0x%p\n", scriptContextPointer);
#endif

    ScriptContext scriptContext(scriptContextPointer);
    executorConsole.StartExecutor(scriptContext);

    return EXIT_SUCCESS;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, onDllAttached, nullptr, 0, nullptr);

    return TRUE;
}
