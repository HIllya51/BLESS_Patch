// dllmain.cpp : 定义 DLL 应用程序的入口点。

#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include<Windows.h>
#include <unordered_map>
#include<thread>
#include<detours.h>  
#pragma comment (lib,"detours.lib") 
#pragma comment (lib,"ntdll.lib") 

auto hDc = GetDC(0);
float rate = 1.25;

auto w = GetSystemMetrics(0);
auto h = GetSystemMetrics(1);
namespace hookfont {
    auto CreateWindowExA_save = CreateWindowExA;
    HWND(WINAPI  CreateWindowExA_HOOK)(
        _In_ DWORD dwExStyle,
        _In_opt_ LPCSTR lpClassName,
        _In_opt_ LPCSTR lpWindowName,
        _In_ DWORD dwStyle,
        _In_ int X,
        _In_ int Y,
        _In_ int nWidth,
        _In_ int nHeight,
        _In_opt_ HWND hWndParent,
        _In_opt_ HMENU hMenu,
        _In_opt_ HINSTANCE hInstance,
        _In_opt_ LPVOID lpParam) {
        return CreateWindowExA_save(dwExStyle, lpClassName, lpWindowName, 0x80cf0800, 0, 0, rate* 640, rate*480, hWndParent, hMenu, hInstance, lpParam);

    }
    auto ChangeDisplaySettingsA_save = ChangeDisplaySettingsA;
    
    LONG (WINAPI ChangeDisplaySettingsA_hook)(
            _In_opt_ DEVMODEA* lpDevMode,
            _In_ DWORD dwFlags) {
        lpDevMode[0].dmPelsWidth = w;
        lpDevMode[0].dmPelsHeight = h;
        return ChangeDisplaySettingsA_save(lpDevMode, dwFlags);

    }
    auto GetDriveTypeA_save = GetDriveTypeA;
    UINT WINAPI  GetDriveTypeA_HOOK(
            _In_opt_ LPCSTR lpRootPathName
        ) {
        auto _ = GetDriveTypeA_save(lpRootPathName);
        if (_ != 5) {
            _ = 5;
        }
        return _;
    }
    void hookfont() {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)CreateWindowExA_save, CreateWindowExA_HOOK);
        DetourAttach(&(PVOID&)ChangeDisplaySettingsA_save, ChangeDisplaySettingsA_hook);
        DetourAttach(&(PVOID&)GetDriveTypeA_save, GetDriveTypeA_HOOK);
        
        DetourTransactionCommit();
    }
}
 
void StartHook(_In_ HINSTANCE hInstance) {

    hookfont::hookfont();

}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        StartHook(hModule);
    case DLL_THREAD_ATTACH:

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

