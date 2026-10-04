#pragma once
#include <Windows.h>
#include <vector>
#include <string> 
#include <iostream>
#include <mutex>
#include <future>
#include <TlHelp32.h>
#include <tchar.h>
//#include <globals.h>
#include "SoundPlayer.h"
#include "Notifications.h"
extern CNotifications notificationManager;

class Memory1
{

public:

	const char* GetEmulatorRunning() {
		if (GetPid("HD-Player.exe") != 0)
			return "HD-Player.exe";

		else if (GetPid("HD-Player") != 0)
			return "HD-Player";

		else if (GetPid("MEmuHeadless.exe") != 0)
			return "MEmuHeadless.exe";

		else if (GetPid("LdVBoxHeadless.exe") != 0)
			return "LdVBoxHeadless.exe";

		else if (GetPid("AndroidProcess.exe") != 0)
			return "AndroidProcess.exe";

		else if (GetPid("Nox.exe") != 0)
			return "Nox.exe";
	}
    const char* targetProcessName = "HD-Player.exe";
	void ReWrite(std::string type, DWORD_PTR dwStartRange, DWORD_PTR dwEndRange, BYTE* Search, BYTE* Replace)
	{
		if (!AttackProcess(GetEmulatorRunning()))
            Beep(0, 0);

		bool Status = ReplacePattern(dwStartRange, dwEndRange, Search, Replace, true);
		if (Status)
            Beep(0, 0);
		else
            Beep(0, 0);

		CloseHandle(ProcessHandle);
	}




    HANDLE GetProcessHandle(const char* processName) {
        HANDLE hProcess = NULL;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create snapshot." << std::endl;
            return NULL;
        }

        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (strcmp(pe32.szExeFile, processName) == 0) {
                    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }

        CloseHandle(hSnapshot);
        return hProcess;
    }

    std::vector<DWORD_PTR> AoBScan(HANDLE hProcess, const std::vector<BYTE>& pattern) {
        std::vector<DWORD_PTR> results;
        MEMORY_BASIC_INFORMATION mbi;
        DWORD_PTR baseAddress = 0;
        DWORD_PTR maxAddress = 0x00007fffffffffff;

        while (baseAddress < maxAddress && VirtualQueryEx(hProcess, (LPCVOID)baseAddress, &mbi, sizeof(mbi))) {
            if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_READWRITE)) {
                std::vector<BYTE> buffer(mbi.RegionSize);
                SIZE_T bytesRead;
                if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                    for (size_t i = 0; i < bytesRead - pattern.size(); ++i) {
                        bool found = true;
                        for (size_t j = 0; j < pattern.size(); ++j) {
                            if (pattern[j] != '?' && pattern[j] != buffer[i + j]) {
                                found = false;
                                break;
                            }
                        }
                        if (found) {
                            results.push_back((DWORD_PTR)mbi.BaseAddress + i);
                        }
                    }
                }
            }
            baseAddress += mbi.RegionSize;
        }
        return results;
    }


    bool WriteMemory(HANDLE hProcess, DWORD_PTR address, const std::vector<BYTE>& data) {
        SIZE_T bytesWritten;
        return WriteProcessMemory(hProcess, (LPVOID)address, data.data(), data.size(), &bytesWritten);
    }


    


 






   


    
	void wallhackbypass()
	{
      //("Wall Hack Bypass Applying...", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(252, 232, 3)));

		if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

		bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
			new BYTE[]{ 0x3C, 0xFF, 0x2F, 0xE1, 0xCC, 0xA0, 0x94, 0xE5, 0x00, 0x00, 0x5A, 0xE3, 0x0F, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x58, 0xE3, 0x01, 0x00, 0x00, 0x1A, 0x00, 0x00 },
			new BYTE[]{ 0x00, 0xF0, 0x20, 0xE3 }, true);
		

		if (st)
		{
          //("Wall Hack Bypass Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
		}
		else
		{
          //("Wall Hack Bypass Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
           
            Beep(300, 300);
		}

		CloseHandle(ProcessHandle);
	}

    void aimfov180()
    {
      //("Aim Fov 280 Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x70, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x0A, 0xD7, 0xA3, 0x3B, 0x0A, 0xD7, 0xA3, 0x3B, 0x8F, 0xC2, 0x75, 0x3D },
            new BYTE[]{ 0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x70, 0x42 }, true);


        if (st)
        {
          //("Aim Fov 280 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Aim Fov 280 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }



    std::vector<BYTE> aim2x = { 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x03, 0x00, 0x00, 0x00, 0xEC, 0x51, 0xB8, 0x3D, 0xCD, 0xCC, 0x4C, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x00 };

    void aimbot2x() {
      //("Aim Fov 2X Applying...", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(252, 232, 3)));
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
          //("Aim Fov 2X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            return;
        }

        auto scanResults = AoBScan(hProcess, aim2x);
        if (scanResults.empty()) {
          //("Aim Fov 2X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            {
                DWORD_PTR targetAddress = address + 0x1E;
                DWORD newValue = 0x7fc00000;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                }
                else {
                }
            }
            {
                DWORD_PTR targetAddress = address + 0x22;
                DWORD newValue = 0x40000000;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                  //("Aim Fov 2X Applied!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(150, 255, 123)));
                    Beep(600, 300);
                }
                else {
                  //("Aim Fov 2X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
                    Beep(300, 300);
                }
            }
        }
        CloseHandle(hProcess);
    }

    std::vector<BYTE> aim4x = { 0x3D, 0x05, 0x00, 0x00, 0x00, 0x29, 0x5C, 0x8F, 0x3D, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xF0, 0x41, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x00, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xD0, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x54, 0x00 };

    void aimbot4x() {
      //("Aim Fov 4X Applying...", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(252, 232, 3)));
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
          //("Aim Fov 4X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            return;
        }

        auto scanResults = AoBScan(hProcess, aim4x);
        if (scanResults.empty()) {
          //("Aim Fov 4X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            {
                DWORD_PTR targetAddress = address + 0x19;
                DWORD newValue = 0x7fc00000;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                  //("Aim Fov 4X Applied!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(150, 255, 123)));
                    Beep(600, 300);
                }
                else {
                  //("Aim Fov 4X Error!!", "HeX Corporation Panel!", 3000, gui->get_clr(ImColor(255, 0, 0)));
                    Beep(300, 300);
                }
            }
        }
        CloseHandle(hProcess);
    }


    void aimtrack2x()
    {
      //("Aim Tracking 2X Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xC7, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x33, 0x33, 0x93, 0x3F, 0x8F, 0xC2, 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x03, 0x00, 0x00, 0x00, 0xEC, 0x51, 0xB8, 0x3D, 0xCD, 0xCC, 0x4C, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x01 },
            new BYTE[]{ 0xC7, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x33, 0x33, 0x93, 0x3F, 0x8F, 0xC2, 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x03, 0x00, 0x00, 0x00, 0xEC, 0x51, 0xB8, 0x3D, 0xCD, 0xCC, 0x4C, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x80, 0x5C, 0x01 }, true);


        if (st)
        {
          //("Aim Tracking 2X Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Aim Tracking 2X Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }





    void aimtrack4x()
    {
      //("Aim Tracking 4X Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xC7, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0xCD, 0xCC, 0x8C, 0x3F, 0x8F, 0xC2, 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x05, 0x00, 0x00, 0x00, 0x29, 0x5C, 0x8F, 0x3D, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xF0, 0x41, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x00, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xD0, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x01 },
            new BYTE[]{ 0xC7, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0xCD, 0xCC, 0x8C, 0x3F, 0x8F, 0xC2, 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x05, 0x00, 0x00, 0x00, 0x29, 0x5C, 0x8F, 0x3D, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xF0, 0x41, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x00, 0x3F, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xD0, 0x3F, 0x00, 0x00, 0x80, 0x5C, 0x01 }, true);


        if (st)
        {
          //("Aim Tracking 4X Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Aim Tracking 4X Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }















    void aimtrack8x()
    {
      //("Aim Tracking 8X Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x40, 0xcd, 0xcc, 0x8c, 0x3f, 0x8f, 0xc2, 0xf5, 0x3c, 0xcd, 0xcc, 0xcc, 0x3d, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x41, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x00, 0x3f, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xb0, 0x3f, 0x00, 0x00, 0x80, 0x3f, 0x01, 0x00, 0x00 },
            new BYTE[]{ 0x40, 0xe0, 0xb1, 0xff, 0xff, 0xe0, 0xb1, 0xff, 0xff, 0xe0, 0xb1, 0xff, 0xff, 0xe0, 0xb1, 0xff, 0xff, 0xe0, 0xb1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x41, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x00, 0x3f, 0x33, 0x33, 0x13, 0x40, 0x00, 0x00, 0xb0, 0x3f, 0x00, 0x00, 0x29, 0x5c, 0x01, 0x00, 0x00 }, true);


        if (st)
        {
          //("Aim Tracking 8X Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Aim Tracking 8X Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }




    std::vector<BYTE> aimYsnipers = { 0x41, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x00, 0x00, 0x00 };


    void aimbotAWMY() {
      //("Aim AWM Y Applying...", "HeX Corporation", 5000, gui->get_clr(ImColor(252, 232, 3)));
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
          //("Aim AWM Y Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            return;
        }

        auto scanResults = AoBScan(hProcess, aimYsnipers);
        if (scanResults.empty()) {
          //("Aim AWM Y Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            {
                DWORD_PTR targetAddress = address + 0x1;
                DWORD newValue = 0x7fc00000;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                  //("Aim AWM Y Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
                    Beep(600, 300);
                }
                else {
                  //("Aim AWM Y Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
                    Beep(300, 300);
                }
            }
        }
        CloseHandle(hProcess);
    }


    std::vector<BYTE> aimdelay = { 0xE3, 0x06, 0x00, 0xA0, 0xE1, 0x18, 0xD0, 0x4B, 0xE2, 0x02, 0x8B, 0xBD, 0xEC, 0x70, 0x8C, 0xBD };

    void aimdelay1() {
      
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
         
            Beep(300, 300);
            return;
        }

        auto scanResults = AoBScan(hProcess, aimdelay);
        if (scanResults.empty()) {
          
            Beep(300, 300);
            CloseHandle(hProcess);
            return;
        }
        
        for (DWORD_PTR address : scanResults) {
            {
                DWORD_PTR targetAddress = address + 0x00;
                DWORD newValue = 0xA00006F3;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                    notificationManager.AddMessage("Applying", "Activated Sniper Delay  ...", "✔", ImColor(255, 165, 0)); // Orange
                    Beep(600, 300);
                }
                else {
                    notificationManager.AddMessage("Failed", "Sniper delay Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure
                    Beep(300, 300);
                }
            }
        }
        CloseHandle(hProcess);
    }

    std::vector<BYTE> m82bnsnipers = { 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB, 0x00, 0x00, 0x00 };


    void aimbotM82B() {
      //("Aim M82B Applying...", "HeX Corporation", 5000, gui->get_clr(ImColor(252, 232, 3)));
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
          //("Aim M82B Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            return;
        }

        auto scanResults = AoBScan(hProcess, m82bnsnipers);
        if (scanResults.empty()) {
          //("Aim M82B Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            {
                DWORD_PTR targetAddress = address + 0x4;
                DWORD newValue = 0x00000000;
                std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
                if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                  //("Aim M82B Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
                    Beep(600, 300);
                }
                else {
                  //("Aim M82B Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
                    Beep(300, 300);
                }
            }
        }
        CloseHandle(hProcess);
    }


    void fastswichsniper()
    {
      //("Sniper Fast Switch Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x81, 0x95, 0xE3, 0x3F, 0x0A, 0xD7, 0xA3, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x43, 0x00, 0x00, 0x8C, 0x42, 0x00, 0x00, 0xB4, 0x42, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x34, 0x42, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F },
            new BYTE[]{ 0x81, 0x95, 0xE3, 0x3F, 0x0A, 0xD7, 0xA3, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x43, 0x00, 0x00, 0x8C, 0x42, 0x00, 0x00, 0xB4, 0x42, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0x51, 0xB8, 0x3D, 0x8F, 0xC2, 0xF5, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x34, 0x42, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F }, true);


        bool st1 = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x3F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x34, 0x42, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xCD, 0xCC, 0x4C, 0x3F, 0xCD, 0xCC, 0x8C, 0x3F, 0x00, 0x00, 0x80 },
            new BYTE[]{ 0x3F, 0x00, 0x00, 0x80, 0x3E, 0xEC, 0x51, 0xB8, 0x3D }, true);

        bool st2 = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xFF, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x0A, 0xD7, 0xA3, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x43, 0x00, 0x00, 0x8C, 0x42, 0x00, 0x00, 0xB4, 0x42, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x34, 0x42, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x33, 0x33, 0x33, 0x3F, 0x9A, 0x99, 0x99, 0x3F, 0x00, 0x00, 0x80 },
            new BYTE[]{ 0xFF, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x8F, 0xC2, 0xF5, 0x3F, 0x0A, 0xD7, 0xA3, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x43, 0x00, 0x00, 0x8C, 0x42, 0x00, 0x00, 0xB4, 0x42, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x80, 0x3E, 0xEC, 0x51, 0xB8, 0x3D, 0x8F, 0xC2, 0xF5, 0x3C }, true);


        if (st)
        {
          //("Sniper Fast Switch Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Sniper Fast Switch Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }

    std::vector<BYTE> aobPattern4 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8E, 0x03, 0x00, 0xEE, 0x90, 0x03, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0xCD, 0xCC, 0x8C, 0x3F, 0x8F, 0xC2, 0xF5, 0x3C, 0xCD, 0xCC, 0xCC, 0x3D, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x41 };


    void sniperScopeFov()
    {
        notificationManager.AddMessage("Applying", "Activating Sniper Scope...", "⌛", ImColor(255, 165, 0)); // Orange for "Applying"
        Activating();

        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
            notificationManager.AddMessage("Failed", "Sniper Scope Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure
           
            return;
        }

        auto scanResults = AoBScan(hProcess, aobPattern4);
        if (scanResults.empty()) {
          //("Sniper Scope Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));

            
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            DWORD_PTR targetAddress = address + 0x2A;
            DWORD newValue = 0xFFE0;
            std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
            if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                notificationManager.AddMessage("Success", "Sniper Scope Activated!", "✔", ImColor(0, 255, 0)); // Green for success
                Activado();
            }
            else {
                notificationManager.AddMessage("Failed", "Sniper Scope Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure
            
                failed();

            }
        }

        CloseHandle(hProcess);
    }

    std::vector<BYTE> aobPattern44 = { 0xED, 0x10, 0x0A, 0x18, 0xEE, 0x04, 0x8B, 0xBD, 0xEC, 0xF0, 0x88, 0xBD, 0xE8, 0x00, 0x00, 0x55 };


    void norec()
    {
        notificationManager.AddMessage("Applying", "Activating camera uppar...", "⌛", ImColor(255, 165, 0)); // Orange for "Applying"
        Activating();

        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
            notificationManager.AddMessage("Failed", "camera uppar Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure

            return;
        }

        auto scanResults = AoBScan(hProcess, aobPattern44);
        if (scanResults.empty()) {
            //("Sniper Scope Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));


            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            DWORD_PTR targetAddress = address + 0x01;
            DWORD newValue = 0xE3430EFF;
            std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
            if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                notificationManager.AddMessage("Success", "camera uppar Activated!", "✔", ImColor(0, 255, 0)); // Green for success
                Activado();
            }
            else {
                notificationManager.AddMessage("Failed", "camera uppar Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure

                failed();

            }
        }

        CloseHandle(hProcess);
    }


  
    void ultraswitchall()
    {
      //("Ultra Fast Switch Gun Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x01, 0x50, 0x00, 0x43, 0x05, 0x00, 0xA0, 0xE1, 0x04, 0x8B, 0xBD, 0xEC, 0x30, 0x88, 0xBD, 0xE8, 0xEB, 0x37, 0x35, 0x07, 0xB8, 0x68, 0x1B, 0x07, 0xC3, 0x37, 0x35, 0x07, 0x9C, 0xF9, 0x1A, 0x07, 0x70, 0xF9, 0x1A, 0x07, 0x30, 0x48, 0x2D, 0xE9, 0x08, 0xB0, 0x8D, 0xE2, 0x00, 0x40, 0xA0, 0xE1 },
            new BYTE[]{ 0x00 }, true);


        if (st)
        {
          //("Ultra Fast Switch Gun Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Ultra Fast Switch Gun Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }

    struct AimbotPatch {
        std::vector<BYTE> aobPattern;
        size_t offset;
        DWORD value;
    };

    void bypass() {
        // Example patterns and addresses (adjust accordingly)
        std::vector<AimbotPatch> patches = {
            {
                // AOB 1
                { 0x1c, 0x00, 0x85, 0xe5, 0x00, 0x70, 0x94, 0xe5, 0xc0, 0x50, 0x96, 0xe5, 0x00, 0x00, 0x57, 0xe3, 0x01, 0x00, 0x00, 0x1a },
                0x00,
                0xE585001D
            },
            {
                // AOB 2
                { 0x28, 0x01, 0x87, 0xe5, 0x00, 0x70, 0x94, 0xe5, 0x01, 0x10, 0x9f, 0xe7, 0x00, 0x00, 0x91, 0xe5, 0xbf, 0x10, 0xd0, 0xe5 }, // <- Replace with actual second AOB
                0x00,
                0xE587012F // Example value: 3.0f
            },
            {
                // AOB 3
                { 0x3C, 0x51, 0x87, 0xE5, 0x00, 0x70, 0x94, 0xE5, 0xCC, 0x50, 0x96, 0xE5, 0x00, 0x00, 0x57, 0xE3, 0x01, 0x00, 0x00, 0x1A }, // <- Replace with actual third AOB
                0x00,
                0xE5875142 // Example value: 1.0f
            }
        };

        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
            notificationManager.AddMessage("Applying", "Activating bypass...", "⌛", ImColor(255, 165, 0)); // Orange for "Applying"
            Activating();
            return;
        }

        for (const auto& patch : patches) {
            auto results = AoBScan(hProcess, patch.aobPattern);
            if (results.empty()) {
                Beep(300, 300); // Error tone
                continue;
            }

            for (DWORD_PTR address : results) {
                DWORD_PTR targetAddress = address + patch.offset;
               
                std::vector<BYTE> valueBytes(reinterpret_cast<BYTE*>(const_cast<DWORD*>(&patch.value)),
                    reinterpret_cast<BYTE*>(const_cast<DWORD*>(&patch.value)) + sizeof(patch.value));


                if (WriteMemory(hProcess, targetAddress, valueBytes)) {
                    notificationManager.AddMessage("Success", "bypass Activated!", "✔", ImColor(0, 255, 0)); // Green for success
                    Activado();
                }
                else {
                    notificationManager.AddMessage("Failed", "bypass Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure
                    failed();
                }
            }
        }

        CloseHandle(hProcess);
    }

    std::vector<BYTE> aobPattern11 = { 0x50, 0x40, 0x33, 0x33, 0xB3 };

    void fastreload()
    {
    


        notificationManager.AddMessage("Applying", "Activating Speed External..", "⌛", ImColor(255, 165, 0)); // Orange for "Applying"

        Activating();
        HANDLE hProcess = GetProcessHandle(targetProcessName);
        if (!hProcess) {
          
            return;
        }

        auto scanResults = AoBScan(hProcess, aobPattern11);
        if (scanResults.empty()) {
            notificationManager.AddMessage("Failed", "Speed External Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure

           
            CloseHandle(hProcess);
            return;
        }

        for (DWORD_PTR address : scanResults) {
            DWORD_PTR targetAddress = address + 0x04;
            DWORD newValue = 0xBA;
            std::vector<BYTE> newValueBytes(reinterpret_cast<BYTE*>(&newValue), reinterpret_cast<BYTE*>(&newValue) + sizeof(newValue));
            if (WriteMemory(hProcess, targetAddress, newValueBytes)) {
                notificationManager.AddMessage("Success", "Speed External Activated!", "✔", ImColor(0, 255, 0)); // Green for success
                Activado();
            }
            else {
                notificationManager.AddMessage("Failed", "Speed External Activation Failed!", "❌", ImColor(255, 0, 0)); // Red for failure
                failed();
               

            }
        }

        CloseHandle(hProcess);
    }


    void Fastfire()
    {
      //("Fast Fire Gun Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xCD, 0xCC, 0x4C, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F },
            new BYTE[]{ 0x00, 0x00, 0x80, 0xBF }, true);


        if (st)
        {
          //("Fast Fire Gun Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Fast Fire Gun Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    void Fastfire2()
    {
      //("Fast Fire Gun 2 Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x1E, 0xFF, 0x2F, 0xE1, 0x38, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1, 0x3C, 0x00, 0x90, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1, 0x3C, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1 },
            new BYTE[]{ 0x1E, 0xFF, 0x2F, 0xE1, 0x38, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1, 0x3C, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1 }, true);


        if (st)
        {
          //("Fast Fire Gun 2 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Fast Fire Gun 2 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }

    void norecoile()
    {
      //("No Recoil Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x7a, 0x44, 0xf0, 0x48, 0x2d, 0xe9, 0x10, 0xb0, 0x8d, 0xe2, 0x02, 0x8b, 0x2d, 0xed, 0x08, 0xd0 },
            new BYTE[]{ 0x7a, 0xff, 0xf0, 0x48, 0x2d, 0xe9, 0x10, 0xb0, 0x8d, 0xe2, 0x02, 0x8b, 0x2d, 0xed, 0x08, 0xd0 }, true);


        if (st)
        {
          //("No Recoil Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("No Recoil Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }

    void magicv1b()
    {
      //("Magic Bullets V1 Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x8C, 0x00, 0xAC, 0xC5, 0x27, 0x37, 0x30, 0x48, 0x2D, 0xE9, 0x01, 0x40, 0xA0, 0xE1, 0x20, 0x10, 0x9F, 0xE5, 0x00, 0x50, 0xA0, 0xE1, 0x1C },
            new BYTE[]{ 0x8C, 0x00, 0xAC, 0xC5, 0x7C, 0x3F, 0x30, 0x48, 0x2D, 0xE9, 0x01, 0x40, 0xA0, 0xE1, 0x20, 0x10, 0x9F, 0xE5, 0x00, 0x00, 0xA0, 0x50, 0x1C }, true);


        if (st)
        {
          //("Magic Bullets V1 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Magic Bullets V1 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    void magicv2b()
    {
      //("Magic Bullets V2 Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x23, 0xAA, 0xA6, 0xB8, 0x46, 0x0A, 0xCD, 0x70 },
            new BYTE[]{ 0x23, 0xAA, 0xA6, 0xB8, 0xB2, 0xF7, 0x1F, 0xA4 }, true);
        bool st1 = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x47, 0x7B, 0x5A, 0xBD, 0xAE, 0x57, 0x66, 0xBB, 0x5C, 0x1F, 0x48, 0xBA, 0x1B, 0xC0, 0xCF, 0x3B, 0x9C, 0xFB, 0x28, 0x3D, 0xA2, 0xB1, 0x17, 0xBD, 0xE4, 0x99, 0x7F, 0x3F, 0x04, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0xFE, 0xFF, 0x7F, 0x3F },
            new BYTE[]{ 0x8D, 0x07, 0x74, 0x3F, 0xAE, 0x57, 0x66, 0xBB, 0x5C, 0x1F, 0x48, 0xBA, 0x1B, 0xC0, 0xCF, 0x3B, 0x9C, 0xFB, 0x28, 0x3D, 0xA2, 0xB1, 0x17, 0xBD, 0xE4, 0x99, 0x7F, 0x3F, 0x00, 0x00, 0x60, 0x41, 0x00, 0x00, 0x60, 0x41, 0x00, 0x00, 0x60, 0x41 }, true);
        bool st2 = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x4C, 0x7B, 0x5A, 0xBD, 0x0A, 0x57, 0x66, 0xBB, 0x1E, 0x21, 0x48, 0xBA, 0x2A, 0xC2, 0xCF, 0x3B, 0x96, 0xFB, 0x28, 0x3D, 0xE8, 0xB1, 0x17, 0xBD, 0xE3, 0x99, 0x7F, 0x3F, 0x04, 0x00, 0x80, 0x3F, 0x01, 0x00, 0x80, 0x3F, 0xFC, 0xFF, 0x7F, 0x3F },
            new BYTE[]{ 0x1B, 0x0E, 0x74, 0x3F, 0xAE, 0x57, 0x66, 0xBB, 0x5C, 0x1F, 0x48, 0xBA, 0x1B, 0xC0, 0xCF, 0x3B, 0x9C, 0xFB, 0x28, 0x3D, 0xA2, 0xB1, 0x17, 0xBD, 0xE4, 0x99, 0x7F, 0x3F, 0x00, 0x00, 0x60, 0x41, 0x00, 0x00, 0x60, 0x41, 0x00, 0x00, 0x60, 0x41 }, true);
        bool st3 = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x10, 0x00, 0x00, 0x00, 0x62, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x5F, 0x00, 0x4C, 0x00, 0x65, 0x00, 0x66, 0x00, 0x74, 0x00, 0x5F, 0x00, 0x57, 0x00, 0x65, 0x00, 0x61, 0x00, 0x70, 0x00, 0x6F, 0x00, 0x6E, 0x00 },
            new BYTE[]{ 0x10, 0x00, 0x00, 0x00, 0x62, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x70, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, true);


        if (st)
        {
          //("Magic Bullets V2 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Magic Bullets V2 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }



    void camerav2()
    {
      //("Camera Hack V2  Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xCD, 0xCC, 0xCC, 0x3E, 0x9A, 0x99, 0x19, 0x3F, 0xCD, 0xCC, 0x4C, 0xBD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x41, 0xCD, 0xCC, 0xCC, 0x3E, 0x00, 0x00, 0x70, 0x42, 0x00, 0x00, 0x70, 0x42 },
            new BYTE[]{ 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x19, 0x3F, 0x00, 0x00, 0x80, 0x3F }, true);


        if (st)
        {
          //("Camera Hack V2 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Camera Hack V2 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }

    void camerafixed()
    {
      //("Camera Scope Fix Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x00, 0x00, 0xC0, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x42, 0x9A, 0x99, 0x99, 0x3E, 0x0A, 0xD7, 0xA3, 0x3D, 0x9A, 0x99, 0x99, 0x3E, 0x00, 0x00, 0x00, 0x3F, 0x8F, 0xC2, 0xF5, 0x3E, 0xB8, 0x1E, 0x05, 0x3F, 0x00, 0x00, 0x34, 0x42, 0xCD, 0xCC, 0xCC, 0x3D, 0x66, 0x66, 0x66, 0x3F },
            new BYTE[]{ 0x00, 0x00, 0x00, 0x00, 0x01 }, true);


        if (st)
        {
          //("Camera Scope Fix Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Camera Scope Fix Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    void camerabypass()
    {
      //("Camera Hack Bypass Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x34, 0xFF, 0x2F, 0xE1, 0x09, 0x00, 0xA0, 0xE1, 0x1E, 0x89, 0xFE, 0xEB, 0x00, 0x60, 0xA0, 0xE1 },
            new BYTE[]{ 0xE3, 0x20, 0xF0, 0x00 }, true);


        if (st)
        {
          //("Camera Hack Bypass Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Camera Hack Bypass Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    
    void wallbyass()
    {
      //("Wall Hack Bypass Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x3C, 0xFF, 0x2F, 0xE1, 0xCC, 0xA0, 0x94, 0xE5, 0x00, 0x00, 0x5A, 0xE3, 0x0F, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x58, 0xE3, 0x01, 0x00, 0x00, 0x1A, 0x00, 0x00 },
            new BYTE[]{ 0x00, 0xF0, 0x20, 0xE3 }, true);


        if (st)
        {
          //("Wall Hack Bypass Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Wall Hack Bypass Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }



   

    void vbadgon()
    {
      //("V Badge Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x70, 0x4C, 0x2D, 0xE9, 0x10, 0xB0, 0x8D, 0xE2, 0x01, 0x40, 0xA0, 0xE1, 0x00, 0x50, 0xA0, 0xE1, 0xF3, 0x05, 0x05, 0xE3 },
            new BYTE[]{ 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 }, true);


        if (st)
        {

          //("V Badge Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("V Badge Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }



   



   

    void aimcolr1()
    {
      //("Aim Color + HD Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0xF5, 0xF4, 0x74, 0x3E, 0xD1, 0xD0, 0x50, 0x3E, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F },
            new BYTE[]{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0xF5, 0xF4, 0x74, 0x3E, 0xD1, 0xD0, 0x50, 0x3E, 0x00, 0x00, 0xFF, 0x3F, 0x00, 0x00, 0x80, 0x3F }, true);


        if (st)
        {

          //("Aim Color + HD Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Aim Color + HD Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }



    void antena1()
    {
      //("Antena Head Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x00, 0x00, 0x80, 0x3f, 0xac, 0xe5, 0x22, 0xbf, 0xc5, 0x1f, 0xa0, 0xbc, 0x0c, 0x6c, 0x45, 0xbf },
            new BYTE[]{ 0xcd, 0xac, 0xee, 0x44, 0xac, 0xe5, 0x22, 0xbf, 0xc5, 0x1f, 0xa0, 0xbc, 0x0c, 0x6c, 0x45, 0xbf }, true);


        if (st)
        {

          //("Antena Head Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Antena Head Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    void night()
    {
      //("Night Mode Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xBD, 0x37, 0x86, 0x35, 0x00, 0x00, 0x51, 0xE3, 0x04, 0x10, 0x91, 0x15, 0x00, 0x10, 0xA0, 0x03, 0x24, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1, 0x24, 0x00, 0x80, 0xE2, 0xCA, 0xE7, 0xF5, 0xEA, 0x30, 0x10, 0x9F, 0xE5, 0x30, 0x20, 0x9F, 0xE5, 0x01, 0x10, 0x8F, 0xE0, 0x10, 0x00, 0x81, 0xE5 },
            new BYTE[]{ 0xBD, 0x37, 0x86, 0xFE, 0x00, 0x00, 0x51, 0xE3, 0x04, 0x10, 0x91, 0x15, 0x00, 0x10, 0xA0, 0x03, 0x24, 0x10, 0x80, 0xE5, 0x1E, 0xFF, 0x2F, 0xE1, 0x24, 0x00, 0x80, 0xE2, 0xCA, 0xE7, 0xF5, 0xEA, 0x30, 0x10, 0x9F, 0xE5, 0x30, 0x20, 0x9F, 0xE5, 0x01, 0x10, 0x8F, 0xE0, 0x10, 0x00, 0x81, 0xE5 }, true);


        if (st)
        {

          //("Night Mode Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Night Mode Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }


    void widec()
    {
      //("Wide Mode Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0xdb, 0x0f, 0x49, 0x40, 0x10, 0x2a, 0x00, 0xee, 0x00, 0x10, 0x80, 0xe5, 0x10, 0x3a, 0x01, 0xee, 0x14, 0x10, 0x80, 0xe5, 0x00, 0x2a, 0x30, 0xee, 0x00, 0x10, 0x00, 0xe3, 0x41, 0x3a, 0x30, 0xee },
            new BYTE[]{ 0x00, 0x00, 0xa0, 0x40, 0x10, 0x2a, 0x00, 0xee, 0x00, 0x10, 0x80, 0xe5, 0x10, 0x3a, 0x01, 0xee, 0x14, 0x10, 0x80, 0xe5, 0x00, 0x2a, 0x30, 0xee, 0x00, 0x10, 0x00, 0xe3, 0x41, 0x3a, 0x30, 0xee }, true);


        if (st)
        {

          //("Wide Mode Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("Wide Mode Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }




    void anticheat1()
    {
      //("AntiCheat V5 Applying...", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(252, 232, 3)));

        if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 300);

        bool st = ReplacePattern(0x0L, 0x00007fffffffffff,
            new BYTE[]{ 0x00, 0x48, 0x2D, 0xE9, 0x0D, 0xB0, 0xA0, 0xE1, 0x18, 0xD0, 0x4D, 0xE2, 0x04, 0x00, 0x0B, 0xE5, 0x08, 0x10, 0x0B, 0xE5, 0x0C, 0x20, 0x8D, 0xE5, 0x04, 0x00, 0x1B, 0xE5, 0x08, 0x10, 0x1B, 0xE5, 0x08, 0x00, 0x8D, 0xE5, 0x16, 0x00, 0x00, 0xEB, 0x08, 0x10, 0x1B, 0xE5 },
            new BYTE[]{ 0x00, 0x48, 0x2D, 0xE9, 0x0D, 0xB0, 0xA0, 0xE1, 0x18, 0xD0, 0x4D, 0xE2, 0x04, 0x00, 0x0B, 0xE5, 0x08, 0x10, 0x0B, 0xE5, 0x0C, 0x20, 0x8D, 0xE5, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x1B, 0xE5, 0x08, 0x00, 0x8D, 0xE5, 0x16, 0x00, 0x00, 0xEB, 0x08, 0x10, 0x1B, 0xE5 }, true);
       

        if (st)
        {

          //("AntiCheat V5 Applied!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(150, 255, 123)));
            Beep(600, 300);
        }
        else
        {
          //("AntiCheat V5 Error!!", "HeX Corporation Panel!", 5000, gui->get_clr(ImColor(255, 0, 0)));
            Beep(300, 300);
        }

        CloseHandle(ProcessHandle);
    }
























	void deWrite(std::string type, DWORD_PTR dwStartRange, DWORD_PTR dwEndRange, BYTE* Search, BYTE* Replace)
	{
		if (!AttackProcess(GetEmulatorRunning()))
            Beep(300, 900);

		bool Status = ReplacePattern(dwStartRange, dwEndRange, Search, Replace, true);
		if (Status)
            Beep(300, 900);
		else
            Beep(300, 900);

		CloseHandle(ProcessHandle);
	}

	DWORD ProcessId = 0;
	HANDLE ProcessHandle;

	typedef struct _MEMORY_REGION
	{
		DWORD_PTR dwBaseAddr;
		DWORD_PTR dwMemorySize;
	}MEMORY_REGION;

	int GetPid(const char* procname)
	{

		if (procname == NULL)
			return 0;
		DWORD pid = 0;
		DWORD threadCount = 0;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe;

		pe.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnap, &pe);
		while (Process32Next(hSnap, &pe)) {
			if (_tcsicmp(pe.szExeFile, procname) == 0) {
				if ((int)pe.cntThreads > threadCount) {
					threadCount = pe.cntThreads;

					pid = pe.th32ProcessID;

				}
			}
		}
		return pid;
	}
	BOOL AttackProcess(const char* procname)
	{
		DWORD ProcId = GetPid(procname);
		if (ProcId == 0)
			return false;

		ProcessId = ProcId;
		ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessId);
		return ProcessHandle != nullptr;
	}

    bool ChangeProtection(ULONG Address, size_t size, DWORD NewProtect, DWORD& OldProtect)
    {
        return VirtualProtectEx(ProcessHandle, (LPVOID)Address, size, NewProtect, &OldProtect);;
    }


    bool ReplacePattern(DWORD_PTR dwStartRange, DWORD_PTR dwEndRange, BYTE* SearchAob, BYTE* ReplaceAob, bool ForceWrite = false)
    {
        int RepByteSize = _msize(ReplaceAob);
        if (RepByteSize <= 0) return false;
        std::vector<DWORD_PTR> foundedAddress;
        FindPattern(dwStartRange, dwEndRange, SearchAob, foundedAddress);
        if (foundedAddress.empty())
            return false;

        OutputDebugStringA(std::to_string(foundedAddress.size()).c_str());

        DWORD OldProtect;
        for (int i = 0; i < foundedAddress.size(); i++)
        {
            ChangeProtection(foundedAddress[i], RepByteSize, PAGE_EXECUTE_READWRITE, OldProtect);
            WriteProcessMemory(ProcessHandle, (LPVOID)foundedAddress[i], ReplaceAob, RepByteSize, 0);
        }

        return true;
    }


    bool FindPattern(DWORD_PTR StartRange, DWORD_PTR EndRange, BYTE* SearchBytes, std::vector<DWORD_PTR>& AddressRet) {
        MEMORY_BASIC_INFORMATION mbi;
        mbi.RegionSize = 0x1000;
        DWORD_PTR dwAddress = StartRange;
        DWORD_PTR nSearchSize = _msize(SearchBytes);

        std::vector<MEMORY_REGION> m_vMemoryRegion;

        // Collect all memory regions
        while (VirtualQueryEx(ProcessHandle, (LPCVOID)dwAddress, &mbi, sizeof(mbi)) && (dwAddress < EndRange) && ((dwAddress + mbi.RegionSize) > dwAddress)) {
            if ((mbi.State == MEM_COMMIT) && ((mbi.Protect & PAGE_GUARD) == 0) && (mbi.Protect != PAGE_NOACCESS) && ((mbi.AllocationProtect & PAGE_NOCACHE) != PAGE_NOCACHE)) {
                MEMORY_REGION mData = { (DWORD_PTR)mbi.BaseAddress, mbi.RegionSize };
                m_vMemoryRegion.push_back(mData);
            }
            dwAddress = (DWORD_PTR)mbi.BaseAddress + mbi.RegionSize;
        }

        std::mutex mtx;

        auto processRegion = [&](MEMORY_REGION mData) {
            BYTE* pCurrMemoryData = new BYTE[mData.dwMemorySize];
            ZeroMemory(pCurrMemoryData, mData.dwMemorySize);
            DWORD_PTR dwNumberOfBytesRead = 0;

            // Read process memory
            ReadProcessMemory(ProcessHandle, (LPCVOID)mData.dwBaseAddr, pCurrMemoryData, mData.dwMemorySize, &dwNumberOfBytesRead);
            if ((int)dwNumberOfBytesRead > 0) {
                DWORD_PTR dwOffset = 0;
                int iOffset = Memfind(pCurrMemoryData, dwNumberOfBytesRead, SearchBytes, nSearchSize);
                while (iOffset != -1) {
                    dwOffset += iOffset;
                    DWORD_PTR firstByteAddress = dwOffset + mData.dwBaseAddr;

                    std::lock_guard<std::mutex> lock(mtx);
                    AddressRet.push_back(firstByteAddress);

                    dwOffset += nSearchSize;
                    iOffset = Memfind(pCurrMemoryData + dwOffset, dwNumberOfBytesRead - dwOffset - nSearchSize, SearchBytes, nSearchSize);
                }
            }

            delete[] pCurrMemoryData;
            };

        // Launch threads to process memory regions concurrently
        std::vector<std::future<void>> futures;
        for (const auto& region : m_vMemoryRegion) {
            futures.push_back(std::async(std::launch::async, processRegion, region));
        }


        for (auto& fut : futures)
        {
            fut.get();
        }

        return true;
    }

    int Memfind(BYTE* buffer, DWORD_PTR dwBufferSize, BYTE* bstr, DWORD_PTR dwStrLen)
    {
        if (dwBufferSize < 0)
        {
            return -1;
        }
        DWORD_PTR  i, j;
        for (i = 0; i < dwBufferSize; i++)
        {
            for (j = 0; j < dwStrLen; j++)
            {
                if (buffer[i + j] != bstr[j] && bstr[j] != '?')
                    break;

            }
            if (j == dwStrLen)
                return i;
        }
        return -1;
    }


    static int findMyProc(const char* procname) {
        if (procname == NULL)
            return 0;
        DWORD pid = 0;
        DWORD threadCount = 0;

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe;

        pe.dwSize = sizeof(PROCESSENTRY32);
        Process32First(hSnap, &pe);
        while (Process32Next(hSnap, &pe)) {
            if (_tcsicmp(pe.szExeFile, procname) == 0) {
                if ((int)pe.cntThreads > threadCount) {
                    threadCount = pe.cntThreads;

                    pid = pe.th32ProcessID;

                }
            }
        }
        return pid;
    }
};
