#ifndef BLINKWIDGET_H
#define BLINKWIDGET_H

#include "precompiled.h"
#include <string>
#include <vector>

class blinkWidget : public wl::window_control
{
    wkeWebView webview_;

public:
    blinkWidget();

    void readFile(const std::string &path, std::vector<char>& data);
    void resize(const SIZE &size);

private:
    void OnWkeInit();

    void KeyEventHandler();
    void PaintEventHandler();
    void MouseEventHandler();
    void FocusEventHandler();

};

#endif // BLINKWIDGET_H
