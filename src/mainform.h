#ifndef MAINFORM_H
#define MAINFORM_H

#include "precompiled.h"
#include "shadowform.h"
#include "gdiplushelper.h"
#include "blinkwidget.h"

class MainForm final : public wl::window_main
{
    GdiPlusHelper gdiplus_;
    ShadowForm shadow_;

    blinkWidget webview_;

public:
    MainForm();

private:
    bool isWindowMove(const POINT &pt);
    bool isWindowZoom(const SIZE &size);
};

#endif // MAINFORM_H
