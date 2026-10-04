#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // For std::transform
#include <locale>    // For std::tolower
#include <Psapi.h>

typedef BOOL(WINAPI* LPWRITEPROCESSMEMORY)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);

BOOL PatchLoadLibraryA()
{
    HMODULE kernelModule = GetModuleHandleA("kernel32.dll");
    LPWRITEPROCESSMEMORY fnWriteProcessMemory = (LPWRITEPROCESSMEMORY)GetProcAddress(kernelModule, "WriteProcessMemory");
    if (!fnWriteProcessMemory)
    {
        std::cerr << "Failed to get WriteProcessMemory function address" << std::endl;
        return FALSE;
    }

    LPVOID loadLibraryA = GetProcAddress(kernelModule, "LoadLibraryA");
    if (!loadLibraryA)
    {
        std::cerr << "Failed to get LoadLibraryA function address" << std::endl;
        return FALSE;
    }

    BYTE hookedCode[] = { 0xC2, 0x04, 0x00 };
    SIZE_T bytesWritten;
    BOOL status = fnWriteProcessMemory(GetCurrentProcess(), loadLibraryA, hookedCode, sizeof(hookedCode), &bytesWritten);
    if (status)
        return TRUE;

    std::cerr << "WriteProcessMemory failed" << std::endl;
    return FALSE;
}

BOOL PatchLoadLibraryW()
{
    HMODULE kernelModule = GetModuleHandleA("kernel32.dll");
    LPWRITEPROCESSMEMORY fnWriteProcessMemory = (LPWRITEPROCESSMEMORY)GetProcAddress(kernelModule, "WriteProcessMemory");
    if (!fnWriteProcessMemory)
    {
        std::cerr << "Failed to get WriteProcessMemory function address" << std::endl;
        return FALSE;
    }

    LPVOID loadLibraryW = GetProcAddress(kernelModule, "LoadLibraryW");
    if (!loadLibraryW)
    {
        std::cerr << "Failed to get LoadLibraryW function address" << std::endl;
        return FALSE;
    }

    BYTE hookedCode[] = { 0xC2, 0x04, 0x00 };
    SIZE_T bytesWritten;
    BOOL status = fnWriteProcessMemory(GetCurrentProcess(), loadLibraryW, hookedCode, sizeof(hookedCode), &bytesWritten);
    if (status)
        return TRUE;

    std::cerr << "WriteProcessMemory failed" << std::endl;
    return FALSE;
}

BOOL SetProcessMitigationPolicy(DWORD dwPolicy, PVOID lpBuffer, DWORD dwBufferSize)
{
    typedef BOOL(WINAPI* LPSETPROCESSMITIGATIONPOLICY)(DWORD, PVOID, DWORD);
    HMODULE kernelModule = GetModuleHandleA("kernel32.dll");
    LPSETPROCESSMITIGATIONPOLICY fnSetProcessMitigationPolicy = (LPSETPROCESSMITIGATIONPOLICY)GetProcAddress(kernelModule, "SetProcessMitigationPolicy");
    if (!fnSetProcessMitigationPolicy)
    {
        std::cerr << "Failed to get SetProcessMitigationPolicy function address" << std::endl;
        return FALSE;
    }

    BOOL status = fnSetProcessMitigationPolicy(dwPolicy, lpBuffer, dwBufferSize);
    if (status)
        return TRUE;

    std::cerr << "SetProcessMitigationPolicy failed" << std::endl;
    return FALSE;
}

BOOL BinaryImageSignatureMitigationAntiDllInjection()
{
    PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY policy;
    policy.MicrosoftSignedOnly = 1;
    BOOL status = SetProcessMitigationPolicy(8, &policy, sizeof(policy));
    if (status)
        return TRUE;

    std::cerr << "BinaryImageSignatureMitigationAntiDllInjection failed" << std::endl;
    return FALSE;
}

BOOL IsInjectedLibrary()
{
    BOOL isMalicious = FALSE;
    std::wstring windowsFolder = L"";
    windowsFolder.resize(MAX_PATH);
    GetWindowsDirectoryW(&windowsFolder[0], MAX_PATH);

    std::wstring programDataFolder = windowsFolder;
    programDataFolder.replace(programDataFolder.find(L"\\Windows"), 8, L"\\ProgramData");

    HANDLE hProcess = GetCurrentProcess();
    DWORD cbNeeded;
    std::vector<HMODULE> hModules(1024);
    if (EnumProcessModules(hProcess, &hModules[0], hModules.size() * sizeof(HMODULE), &cbNeeded))
    {
        for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++)
        {
            std::wstring fileName(MAX_PATH, L'\0');
            if (GetModuleFileNameExW(hProcess, hModules[i], &fileName[0], MAX_PATH))
            {
                std::wstring lowerFileName = fileName;
                std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), ::towlower);

                if (lowerFileName.compare(0, windowsFolder.size(), windowsFolder) != 0 &&
                    lowerFileName.compare(0, programDataFolder.size(), programDataFolder) != 0)
                {
                    isMalicious = TRUE;
                }

                if (lowerFileName.compare(0, GetCurrentDirectoryW(MAX_PATH, &lowerFileName[0]), 0) == 0)
                {
                    isMalicious = FALSE;
                }
            }
        }
    }
    return isMalicious;
}

BOOL SetDllLoadPolicy()
{
    PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY policy;
    policy.MicrosoftSignedOnly = 1;
    BOOL status = SetProcessMitigationPolicy(0x10, &policy, sizeof(policy));
    if (status)
        return TRUE;

    std::cerr << "SetDllLoadPolicy failed" << std::endl;
    return FALSE;
}

