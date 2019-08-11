#include "precompiled.h"
#include "mainform.h"
#include "internals/run.h"

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {

    wkeSetWkeDllPath(L"E:\\3rdParty\\extras\\miniblink\\node_v8_4_8.dll");
    wkeInitialize();

    return wl::_wli::run_main<MainForm>(hInst, SW_HIDE);
}
