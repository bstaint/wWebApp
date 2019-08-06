#ifndef BLINKWIDGET_H
#define BLINKWIDGET_H

#include "precompiled.h"
#include <string>
#include <vector>
#include <mutex>
#include "zipui.h"

#define APP_SAFE_URL "mb://"
#define NOT_FOUND_STR "<h2>Not Found</h2>"

class blinkWidget : public wl::window_control
{
    wkeWebView webview_;

    ZipUi zip_;
    static std::once_flag s_ready_flag;

    int cursor_type_;

public:
    blinkWidget();

    void resize(const SIZE &size);

private:
    void OnWkeInit();

    void KeyEventHandler();
    void PaintEventHandler();
    void MouseEventHandler();
    void FocusEventHandler();

};

#endif // BLINKWIDGET_H
