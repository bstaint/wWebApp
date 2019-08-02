#ifndef SHADOWFORM_H
#define SHADOWFORM_H

#include "precompiled.h"
#include <gdiplus.h>

class ShadowForm : public wl::window_control
{
    Gdiplus::Image image_;

public:
    ShadowForm();

    void DrawShadowUI();
};

#endif // SHADOWFORM_H
