#include "shadowform.h"
#include <sstream>

using namespace wl;
using namespace Gdiplus;

static const int kShadowWidth = 5;

void ShadowForm::bindWindow(const HWND &hwnd)
{
    hwnd_ = hwnd;
}

ShadowForm::ShadowForm() :
    m_image(L"../res/shadow.png")
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
    POINT pos = { setup.position.x, setup.position.y };
    SIZE size = { setup.size.cx + kShadowWidth, setup.size.cy + kShadowWidth};

    HDC screenDC = GetDC(NULL);

    HDC overlayDC = CreateCompatibleDC(screenDC);
    HBITMAP overlayBitmap = CreateCompatibleBitmap(screenDC, size.cx, size.cy);
    HBITMAP OriBitmap = (HBITMAP)SelectObject(overlayDC, overlayBitmap);

    Graphics g(overlayDC);
    g.DrawImage(&m_image, 0, 0, size.cx, size.cy);

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    auto ret = UpdateLayeredWindow(hwnd(), NULL, &pos, &size, overlayDC,
        &zero, 0, &bf, ULW_ALPHA);
    _ASSERT(ret);

    SelectObject(overlayDC, OriBitmap);
    DeleteObject(overlayBitmap);
    DeleteDC(overlayDC);
}
