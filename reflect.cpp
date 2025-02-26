#include <windows.h>

typedef void (*ReflectiveDllMain)(HINSTANCE, DWORD, LPVOID);

void ReflectiveLoader(HINSTANCE hInstance) {
    // 获取反射 DLL 的入口地址
    ReflectiveDllMain ReflectiveEntry = (ReflectiveDllMain)hInstance;

    // 调用反射 DLL 的入口点
    ReflectiveEntry(hInstance, DLL_PROCESS_ATTACH, NULL);
}
