#ifndef BLINKWIDGET_H
#define BLINKWIDGET_H

#include "precompiled.h"
#include <string>
#include <vector>
#include "zipui.h"

class blinkWidget : public wl::window_control
{
    wkeWebView webview_;

    ZipUi zip_;

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
