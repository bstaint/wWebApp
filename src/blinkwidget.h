#ifndef BLINKWIDGET_H
#define BLINKWIDGET_H

#include "precompiled.h"
#include <string>
#include <vector>
#include <mutex>
#include "zipui.h"

class blinkWidget : public wl::window_control
{
    wkeWebView webview_;

    ZipUi zip_;
    static std::once_flag ready_flag;

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
