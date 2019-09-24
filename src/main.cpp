#include "precompiled.h"
#include "mainform.h"
#include "internals/run.h"
#include "executable.h"

void dllWkeInit()
{
#ifdef _DEBUG
    std::wstring node_dll_path = L"E:\\3rdParty\\extras\\miniblink\\node_v8_4_8.dll";
#else
    std::wstring node_dll_path = wl::executable::get_own_path() + L"\\node_v8_4_8.dll";
#endif
    OutputDebugStringW(node_dll_path.c_str());
    wkeSetWkeDllPath(node_dll_path.c_str());
    wkeInitialize();
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    // 避免_CrtDumpMemoryLeaks报内存泄漏
    dllWkeInit();
    // 防止窗口白屏，等待wke加载完毕之后SW_SHOW
    return wl::_wli::run_main<MainForm>(hInst, SW_HIDE);
}
