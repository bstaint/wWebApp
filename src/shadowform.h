#ifndef SHADOWFORM_H
#define SHADOWFORM_H

#include "precompiled.h"
#include <gdiplus.h>

class ShadowForm : public wl::window_control
{
    Gdiplus::Image image_;

    int width_;
    int height_;

private:
    void DrawShadowUI();

public:
    ShadowForm();

    void move(POINT& pt);
    void zoom(POINT& pt, SIZE &size);
};

#endif // SHADOWFORM_H
