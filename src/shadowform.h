#ifndef SHADOWFORM_H
#define SHADOWFORM_H

#include "precompiled.h"
#include <gdiplus.h>

class ShadowForm : public wl::window_control
{
    Gdiplus::Image m_image;
    HWND hwnd_;

public:
    ShadowForm();

    void bindWindow(const HWND &hwnd);
    void DrawShadowUI();
};

#endif // SHADOWFORM_H
