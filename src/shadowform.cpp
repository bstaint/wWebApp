#include "shadowform.h"
#include <sstream>

using namespace wl;
using namespace Gdiplus;

static const int kShadowWidth = 6;

ShadowForm::ShadowForm() :
    image_(L"../res/shadow.png")
{
    setup.wndClassEx.lpszClassName = L"ShadowFromClass";
    setup.style = ws::POPUP | ws::VISIBLE | ws::DISABLED;
    setup.exStyle = wsx::TRANSPARENT_ | wsx::LAYERED | wsx::TOOLWINDOW | wsx::NOACTIVATE;

    on_message(WM_CREATE, [&](params){
        DrawShadowUI();
        return 0;
    });
}

void ShadowForm::DrawShadowUI()
{
    POINT zero = { 0, 0 };
    POINT pos = setup.position;
    SIZE size = setup.size + kShadowWidth;

    HDC screenDC = GetDC(NULL);

    HDC overlayDC = CreateCompatibleDC(screenDC);
    HBITMAP overlayBitmap = CreateCompatibleBitmap(screenDC, size.cx, size.cy);
    HBITMAP OriBitmap = (HBITMAP)SelectObject(overlayDC, overlayBitmap);

    Graphics g(overlayDC);

    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    g.SetPixelOffsetMode(PixelOffsetModeHalf);

    g.DrawImage(&image_, 0, 0, size.cx, size.cy);
//    g.DrawImage(&image_, setup.position.x + setup.size.cx - 5, 0, 792, 0, 3, setup.size.cy, Gdiplus::Unit::UnitPixel);

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 200, AC_SRC_ALPHA };

    auto ret = UpdateLayeredWindow(hwnd(), NULL, &pos, &size, overlayDC,
        &zero, 0, &bf, ULW_ALPHA);
    _ASSERT(ret);

    SelectObject(overlayDC, OriBitmap);
    DeleteObject(overlayBitmap);
    DeleteDC(overlayDC);
}
