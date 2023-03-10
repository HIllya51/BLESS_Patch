
#include<Windows.h>  
#include<cstring> 
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
int main() {


    STARTUPINFO si;
    memset(&si, 0,
        sizeof(STARTUPINFO)); //��ʼ��si���ڴ���е�ֵ�����memset������
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi; //�ر��������ý���

    if (CreateProcess((L"Bless.EXE"), NULL, NULL,
        NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
        wchar_t dll[] = L"bless_window_hook.dll";
        LPVOID remoteData = VirtualAllocEx(handle, nullptr, (wcslen(dll) + 1) * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(handle, remoteData, dll, (wcslen(dll) + 1) * sizeof(wchar_t), nullptr);
        auto h=CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteData, 0, nullptr);
        WaitForSingleObject(h, INFINITE);
        ResumeThread(pi.hThread);
    }


}