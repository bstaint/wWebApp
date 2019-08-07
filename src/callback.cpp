#include "callback.h"
#include "blinkwidget.h"
#include "precompiled.h"

jsValue exeCallback(jsExecState es, void *param)
{
    assert(param != nullptr);
    HWND hwnd = ::GetParent(static_cast<blinkWidget *>(param)->hwnd());

    jsValue arg0 = jsArg(es, 0);
    const utf8* op = jsToTempString(es, arg0);

    if(strcmp(op, u8"mousedown") == 0) {
        PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        ReleaseCapture();
    } else if(strcmp(op, u8"quit") == 0) {
        PostQuitMessage(0);
    } else if(strcmp(op, u8"minimize") == 0) {
        ShowWindow(hwnd, SW_MINIMIZE);
    }

    return jsUndefined();
}

jsValue urlCallback(jsExecState es, void *param)
{
    assert(param != nullptr);

    jsValue arg0 = jsArg(es, 0);
    const utf8* url = jsToTempString(es, arg0);
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOW);

    return jsUndefined();
}
