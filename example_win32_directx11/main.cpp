
#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <dwmapi.h>
#include "imgui_settings.h"
#include "Auth/auth.hpp"
#include "Auth/json.hpp"
#include "Auth/skStr.h"
#include "Auth/utils.hpp"

#include "icon_font.h"
#include "poppins_font.h"
#include "roboto_font.h"

#include "imgui_freetype.h"

#include <iostream>
#include <Windows.h>

#include <string>

#include "SmartyMem.h"

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "circle_load.h"
#include "minecraft_bg.h"
#include "logo.h"

#include "snow.hpp"
#include <memory.h>

using namespace std;
using namespace KeyAuth;
#define SNOW_LIMIT 20

Memory Smarty;

std::string name = skCrypt("EXTERNAL").decrypt();
std::string ownerid = skCrypt("9n56733Ygl").decrypt();
std::string secret = skCrypt("a98c2444b407409c8c1ce2153fd844a9e337c559b3cf8d2ffdbc2ab3996d9325").decrypt();
std::string version = skCrypt("1.0").decrypt();
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting


KeyAuth::api KeyAuthApp(name, ownerid, secret, version, url);
bool authenticed = false;
std::string MemoryLogs = "Status:";




std::vector<Snowflake::Snowflake> snow;

ID3D11ShaderResourceView* circle_loading = nullptr;
ID3D11ShaderResourceView* minecraft_pic = nullptr;
ID3D11ShaderResourceView* lg = nullptr;

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace ImStyle;

#include <thread>
#include <chrono>
#include <random>


HWND hwnd;
RECT rc;

static DWORD tick_count = GetTickCount();

int ProcId = 0;

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



static int GetPid(const char* procname) {

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

const char* GetEmulatorRunning() {
    if (GetPid("HD-Player.exe") != 0)
        return "HD-Player.exe";

    else if (GetPid("MEmuHeadless.exe") != 0)
        return "MEmuHeadless.exe";

    else if (GetPid("LdVBoxHeadless.exe") != 0)
        return "LdVBoxHeadless.exe";

    else if (GetPid("AndroidProcess.exe") != 0)
        return "AndroidProcess.exe";

    else if (GetPid("aow_exe.exe") != 0)
        return "aow_exe.exe";

    else if (GetPid("Nox.exe") != 0)
        return "Nox.exe";
}

DWORD processId = findMyProc(GetEmulatorRunning());


bool InjectDLL(const char* targetProcessName, const char* dllPath) {
    // Find the process alvo hair name
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {

       // MemoryLogs = "Error in creating the snapshot of the process";
        return false;
    }

    HANDLE hProcess = nullptr;
    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, targetProcessName) == 0) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                break;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    if (hProcess == nullptr) {

        MemoryLogs = "Emulator Not Found";
        return false;
    }


    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pRemoteMemory == nullptr) {

        MemoryLogs = "Error Allocating Remote Memory";
        CloseHandle(hProcess);
        return false;
    }


    WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, nullptr);


    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteMemory, 0, nullptr);
    if (hThread == nullptr)
    {

        MemoryLogs = "Error raising remote thread";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }


    WaitForSingleObject(hThread, INFINITE);


    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    MemoryLogs = "DLL Injected Successfully";

    return true;
}

bool EjectDLL(const char* targetProcessName, const char* dllPath) {

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {

       // MemoryLogs = "Error in creating the snapshot of the process";
        return false;
    }

    HANDLE hProcess = nullptr;
    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, targetProcessName) == 0) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                break;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);

    if (hProcess == nullptr)
    {
        MemoryLogs = "Emulator Not Found";
        return false;
    }


    HMODULE hModule = GetModuleHandleA(dllPath);
    if (hModule == nullptr)
    {

        MemoryLogs = "Error in obtaining the identifier of the DLL module";
        CloseHandle(hProcess);
        return false;
    }

    // Obtain the correctness of the FreeLibrary function
    FARPROC pFreeLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");
    if (pFreeLibrary == nullptr) {

        //MemoryLogs = "FreeLibrary get or right error";
        CloseHandle(hProcess);
        return false;
    }


    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pFreeLibrary, hModule, 0, nullptr);
    if (hThread == nullptr)
    {

        MemoryLogs = "Error raising remote thread";
        CloseHandle(hProcess);
        return false;
    }


    WaitForSingleObject(hThread, INFINITE);

    // Clear resources
    CloseHandle(hThread);
    CloseHandle(hProcess);

    MemoryLogs = "DLL removed with success!";

    return true;
}



void move_window() {

    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::InvisibleButton("Move_detector", ImVec2(window::size.x, window::size.y)));
    if (ImGui::IsItemActive()) {

        GetWindowRect(hwnd, &rc);
        MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, window::size.x, window::size.y, TRUE);
    }

}

void RenderBlur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary("user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 }; // and even works 4,0,155,0 (Acrylic blur)
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}

namespace fonts {
    ImFont* poppins = nullptr;
    ImFont* roboto = nullptr;
    ImFont* icon = nullptr;
}

namespace var {

    namespace login {
        char login[16] = { "" };
        std::string login_succes = "user";
    }

    namespace password {
        char password[16] = { "" };
        std::string pass_succes = "12345";
    }

    namespace Login {
        const char* name = "Login";
        bool click = false;
    }

    namespace panel {
        char ID[26] = {""};
        bool change_id = false;
    }

}

int tabs = 0;

float timer_loading = 0.f;

static int product_list = 0;

bool product_selection = true;

void SmartySecurity()
{
    HWND Stealth;
    AllocConsole();
    Stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Stealth, 0);
}

DWORD win_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)SmartySecurity, nullptr, NULL, nullptr);


    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = L"ImGui";
    wc.lpszClassName = L"Example";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, skCrypt(L"Example").decrypt(), WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (window::size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (window::size.y / 2), window::size.x, window::size.y, NULL, NULL, nullptr, nullptr);

    RenderBlur(hwnd);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // ============================================================================
    // APPLY MODERN DARK RED THEME - Visual-only overhaul
    // ============================================================================
    ImStyle::ApplyModernDarkRedTheme();

    //CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)ScanProcess, nullptr, NULL, nullptr);
    KeyAuthApp.init();

    if (!KeyAuthApp.data.success)
    {
        MessageBoxA(NULL, KeyAuthApp.data.message.c_str(), NULL, NULL);
        Sleep(1500);
        exit(0);
    }

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    fonts::poppins = io.Fonts->AddFontFromMemoryTTF(&poppins, sizeof poppins, 23, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    fonts::roboto = io.Fonts->AddFontFromMemoryTTF(&roboto, sizeof roboto, 45, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    fonts::icon = io.Fonts->AddFontFromMemoryTTF(&ico, sizeof ico, 35, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.136f, 0.113f, 0.254f, 1.00f);

    Snowflake::CreateSnowFlakes(snow, SNOW_LIMIT, 5.f, 20.f, 0, 0, window::size.x, window::size.y, Snowflake::vec3(0.f, 0.005f), IM_COL32(125, 0, 251, 50));

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiContext& g = *GImGui;

            D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
            if (circle_loading == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle, sizeof(circle), &info, pump, &circle_loading, 0);
            if (minecraft_pic == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, minecraft_bg, sizeof(minecraft_bg), &info, pump, &minecraft_pic, 0);
            if (lg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), &info, pump, &lg, 0);

            ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
            ImGui::GetStyle().WindowBorderSize = 0.f;
            ImGui::GetStyle().ItemSpacing = ImVec2(15, 15);
            ImGui::GetStyle().ScrollbarSize = 7.f;


            ImGui::SetNextWindowSize(ImVec2(window::size.x, window::size.y));
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin(skCrypt("KENZO X BYPASS").decrypt(), nullptr, win_flags);
            {

                GetWindowRect(hwnd, &rc);

                GetCursorPos(&mouse);

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(window::size.x, window::size.y), ImGui::GetColorU32(window::background), window::triangle);

                Snowflake::Update(snow, Snowflake::vec3(mouse.x, mouse.y), Snowflake::vec3(rc.left, rc.top));

                const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
                ImGui::GetWindowDrawList()->AddRect(ImVec2(1, 1), ImVec2(window::size.x, window::size.y), ImGui::GetColorU32(ImStyle::window::background), window::triangle, ImDrawFlags_None, 2.f);
                ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, ImVec2(1, 1), ImVec2(window::size.x / 2, window::size.y / 2));

                ImGui::SetCursorPos(ImVec2(window::size.x - (45 * 2), 15));
                ImGui::BeginGroup();
                {

                    if (ImGui::TextButton("B", ImVec2(25, 25))) ShowWindow(hwnd, SW_MINIMIZE);

                    ImGui::SameLine();

                    if (ImGui::TextButton("A", ImVec2(25, 25))) {
                        ShowWindow(hwnd, SW_HIDE);
                        done = true;
                    };

                }
                ImGui::EndGroup();

                static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * ImGui::GetStyle().Alpha);

                if (active_tab == 0) {

                    ImGui::GetWindowDrawList()->AddImage(lg, ImVec2(15, 20), ImVec2(85, 90), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(ImStyle::general_color));

                    ImGui::PushFont(fonts::roboto);
                    ImGui::ShadowText("KENZO X BYPASS", ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::window::shadow_text), 110.f, ImVec2((455 / 2 - (ImGui::CalcTextSize("KENZO X BYPASS").x / 2)), 100.f));
                    ImGui::PopFont();

                    ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), (window::size.y / 2) - (105 / 2) - (ImGui::GetStyle().ItemSpacing.y / 2) * 2));
                    ImGui::BeginGroup();
                    {
                        ImGui::InputTextEx("##0", "Username", var::login::login, 16, ImVec2(280, 35), ImGuiInputTextFlags_None);

                        ImGui::InputTextEx("##1", "Password", var::password::password, 16, ImVec2(280, 35), ImGuiInputTextFlags_Password);

                        ImGui::Spacing();

                        ImGui::TextColored(ImColor(ImStyle::text::have_account), "I Don't Have An Account?");

                        if (ImGui::IsItemClicked()) ShellExecute(NULL, "Open", "https://discord.gg/7adDzMS2Yq", NULL, NULL, SW_SHOW);

                        if (ImGui::Button("Login", ImVec2(280, 35)))
                        {

                            KeyAuthApp.login(var::login::login, var::password::password);
                            if (!KeyAuthApp.data.success)
                            {
                                MessageBoxA(NULL, KeyAuthApp.data.message.c_str(), NULL, NULL);

                            }
                            else tabs = 1; /* */ timer_loading = 0.f;




                            product_selection = true;
                            product_list = 0;
                        };
                    }
                    ImGui::EndGroup();

                    // CONTACTS

                    ImGui::SetCursorPos(ImVec2(window::size.x / 2 - (70 / 2) - 20, window::size.y - 65));

                    ImGui::BeginGroup();
                    {


                        if (ImGui::TextButton("C", ImVec2(35, 35)))
                        {

                            ShellExecute(NULL, "open", "https://discord.gg/TR33pzYGjH", NULL, NULL, SW_SHOWNORMAL);

                            //tabs = 3; /* */ timer_loading = 0.f;

                        }


                        ImGui::SameLine(0, 20);

                        if (ImGui::TextButton("D", ImVec2(35, 35)));

                    }
                    ImGui::EndGroup();

                }
                else if (active_tab == 1)
                {

                    for (int i = 1; i <= 7; i++)
                        if (product_list == i) product_selection = false;

                    ImGui::ImageRotation(circle_loading, ImVec2(90, 90), ImVec2(1, 1), ImVec2(0, 0), ImGui::GetColorU32(ImStyle::general_color), 0.1f);

                    timer_loading += 6.f * ImGui::GetIO().DeltaTime;

                    if (timer_loading > 15)  tabs = 2; /* */ else if (timer_loading > 15) /* */ tabs = 0;

                    ImGui::ShadowText("Loading...", ImGui::GetColorU32(ImStyle::text::have_account), ImGui::GetColorU32(ImStyle::text::have_account), 0.f, ImVec2((window::size.x / 2 - (ImGui::CalcTextSize("Loading...").x / 2)), 200.f));

                }
                else if (active_tab == 2)
                {
                    ImGui::SetCursorPos({ 35, 478 });
                    ImGui::Text(MemoryLogs.c_str());


                    ImGui::SetCursorPos(ImVec2(15, 15));
                    if (ImGui::TextButton("E", ImVec2(25, 25))) tabs = 0;


                    if (product_selection)
                    {
                            
                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - 350 / 2, (window::size.y / 2) - 240 / 2));
                        ImGui::PushFont(fonts::roboto);
                        ImGui::ShadowText("KENZO X BYPASS", ImGui::GetColorU32(ImStyle::text::text_active), ImGui::GetColorU32(ImStyle::window::shadow_panel), 110.f, ImVec2((400 / 2 - (ImGui::CalcTextSize("KENZO X BYPASS").x / 2)), 100.f));
                        ImGui::PopFont();

                        ImGui::RenderTextClipped(ImVec2(85, 145), ImVec2(window::size.x - 85, 200), "GAME :", NULL, NULL, ImVec2(0.0f, 0.5f));
                        ImGui::RenderTextClipped(ImVec2(85, 145), ImVec2(window::size.x - 85, 200), "FREE FIRE", NULL, NULL, ImVec2(1.0f, 0.5f));

                        ImGui::RenderTextClipped(ImVec2(85, 190), ImVec2(window::size.x - 85, 250), "DEVELOPER :", NULL, NULL, ImVec2(0.0f, 0.5f));
                        ImGui::RenderTextClipped(ImVec2(85, 190), ImVec2(window::size.x - 85, 250), "</>KENZO", NULL, NULL, ImVec2(1.0f, 0.5f));

                        ImGui::RenderTextClipped(ImVec2(85, 230), ImVec2(window::size.x - 85, 300), "PRODUCT :", NULL, NULL, ImVec2(0.0f, 0.5f));
                        ImGui::RenderTextClipped(ImVec2(85, 230), ImVec2(window::size.x - 85, 300), "BYPASS", NULL, NULL, ImVec2(1.0f, 0.5f));



                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), 395 - 100));
                        if (ImGui::Button("ANTI CHEAT", ImVec2(280, 35)))
                        {
                            Smarty.AC();

                            //tabs = 3; /* */ timer_loading = 0.f;

                        }


                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), 440 - 100));
                        if (ImGui::Button("EMULATOR BYPASS", ImVec2(280, 35))) 
                        {
                            Smarty.Bypass();

                            //tabs = 3; /* */ timer_loading = 0.f;

                        }


                        /*ImGui::GetBackgroundDrawList()->AddImage(minecraft_pic, ImVec2(0, 0), ImVec2(window::size), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(window::background_pic));*/

                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), 485 - 100));

                        if (ImGui::Button("INTERNET BLOCK", ImVec2(280, 35)))
                        {
                            MemoryLogs = "Internet Blocked";
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In1\" dir=in action=block program=\"%ProgramFiles%\\BlueStacks_nxt\\HD-Player.exe\"", SW_HIDE); //Bluestacks 5
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In1\" dir=out action=block program=\"%ProgramFiles%\\BlueStacks_nxt\\HD-Player.exe\"", SW_HIDE); //Bluestacks 5
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In2\" dir=in action=block program=\"%ProgramFiles%\\BlueStacks\\HD-Player.exe\"", SW_HIDE); //Bluestacks 4
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In2\" dir=out action=block program=\"%ProgramFiles%\\BlueStacks\\HD-Player.exe\"", SW_HIDE); //Bluestacks 4
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In3\" dir=in action=block program=\"%ProgramFiles%\\BlueStacks_msi2\\HD-Player.exe\"", SW_HIDE); //Msi 4
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In3\" dir=out action=block program=\"%ProgramFiles%\\Bluestacks_msi2\\HD-Player.exe\"", SW_HIDE); //Msi 4
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In6\" dir=in action=block program=\"%ProgramFiles%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5 x64
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In6\" dir=out action=block program=\"%ProgramFiles%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5 x64
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In4\" dir=in action=block program=\"%ProgramData%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In4\" dir=out action=block program=\"%ProgramData%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In5\" dir=in action=block program=\"%ProgramFiles(x86)%\\SmartGaGa\\ProjectTitan\\Engine\\ProjectTitan.exe\"", SW_HIDE); //Smart Gaga
                            WinExec("netsh advfirewall firewall add rule name=\"FF Block In5\" dir=out action=block program=\"%ProgramFiles(x86)%\\SmartGaGa\\ProjectTitan\\Engine\\ProjectTitan.exe\"", SW_HIDE); //Smart Gaga

                        }

                        /*ImGui::GetBackgroundDrawList()->AddImage(minecraft_pic, ImVec2(0, 0), ImVec2(window::size), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(window::background_pic));*/

                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), 530 - 100));
                        if (ImGui::Button("INTERNET UNBLOCK", ImVec2(280, 35))) 
                        {
                            MemoryLogs = "Internet Unblock";
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramFiles%\\BlueStacks_nxt\\HD-Player.exe\"", SW_HIDE); //Bluestacks 5
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramFiles%\\BlueStacks\\HD-Player.exe\"", SW_HIDE); //Bluestacks 4
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramFiles%\\BlueStacks_msi2\\HD-Player.exe\"", SW_HIDE); //Msi 4
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramFiles%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5 xxx
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramData%\\BlueStacks_msi5\\HD-Player.exe\"", SW_HIDE); //Msi 5
                            WinExec("netsh advfirewall firewall delete rule name=all program=\"%ProgramFiles(x86)%\\SmartGaGa\\ProjectTitan\\Engine\\ProjectTitan.exe\"", SW_HIDE); //Smart Gaga

                        }
                    }
                    else 
                    {

                        if (product_list == 1)
                        {


                        }
                        else if (product_list == 2) 
                        {


                        }


                        /*ImGui::GetBackgroundDrawList()->AddImage(minecraft_pic, ImVec2(0, 0), ImVec2(window::size), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(window::background_pic));*/

                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), window::size.y - 100));
                        if (ImGui::Button("INJECTED", ImVec2(280, 35))) 
                        {
                            tabs = 3; /* */ timer_loading = 0.f;
                        }

                        //ImGui::GetBackgroundDrawList()->AddImage(minecraft_pic, ImVec2(0, 0), ImVec2(window::size), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(window::background_pic));

                        ImGui::SetCursorPos(ImVec2((window::size.x / 2) - (280 / 2), window::size.y - 100));
                        if (ImGui::Button("INJECTED", ImVec2(280, 35)))
                        {
                            tabs = 3; /* */ timer_loading = 0.f;
                        }

                    }

                }
                else if (active_tab == 3) 
                {

                    ImGui::ImageRotation(circle_loading, ImVec2(90, 90), ImVec2(1, 1), ImVec2(0, 0), ImGui::GetColorU32(ImStyle::general_color), 0.1f);

                    timer_loading += 6.f * ImGui::GetIO().DeltaTime;
                    if (timer_loading > 15) tabs = 4;

                    ImGui::ShadowText("The injection has begun.", ImGui::GetColorU32(ImStyle::text::have_account), ImGui::GetColorU32(ImStyle::text::have_account), 0.f, ImVec2((window::size.x / 2 - (ImGui::CalcTextSize("The injection has begun.").x / 2)), 200.f));

                }
                else if (active_tab == 4)
                {

                    ImGui::SetCursorPos(ImVec2(15, 15));
                    if (ImGui::TextButton("E", ImVec2(25, 25))) tabs = 2;

                    ImGui::ShadowText("Injection was successful!", ImGui::GetColorU32(ImStyle::text::have_account), ImGui::GetColorU32(ImStyle::text::have_account), 0.f, ImVec2((window::size.x / 2 - (ImGui::CalcTextSize("Injection was successful!").x / 2)), 200.f));

                }
                else if (active_tab == 5) 
                {

                    ImGui::SetCursorPos(ImVec2(15, 15));
                    if (ImGui::TextButton("E", ImVec2(25, 25))) tabs = 2;

                    ImGui::ShadowText("Injection was unsuccessful!", ImGui::GetColorU32(ImStyle::text::have_account), ImGui::GetColorU32(ImStyle::text::have_account), 0.f, ImVec2((window::size.x / 2 - (ImGui::CalcTextSize("Injection was unsuccessful!").x / 2)), 200.f));

                }

                ImGui::PopStyleVar();

                move_window();
            }

            ImGui::End();

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
