#include "shadowform.h"
#include <sstream>
#include "str.h"
#include "executable.h"

using namespace wl;
using namespace Gdiplus;

ShadowForm::ShadowForm() :
    image_(nullptr),
    width_(6),
    height_(5)
{
    setup.wndClassEx.lpszClassName = L"ShadowFromClass";
    setup.style = WS_POPUP | WS_VISIBLE | WS_DISABLED;
    setup.exStyle = WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;

#ifdef _DEBUG
    std::wstring path = L"..\\res\\shadow.png";
#else
    std::wstring path = executable::get_own_path() + L"\\res\\shadow.png";
#endif
    image_ = std::make_shared<Gdiplus::Image>(path.c_str());

    on_message(WM_CREATE, [&](params){
        DrawShadowUI();
        return 0;
    });
}

void ShadowForm::DrawShadowUI()
{
    POINT zero = { 0, 0 };
    POINT pos = setup.position;
    SIZE size = {setup.size.cx + width_, setup.size.cy + height_};

    HDC screenDC = GetDC(NULL);

    HDC overlayDC = CreateCompatibleDC(screenDC);
    HBITMAP overlayBitmap = CreateCompatibleBitmap(screenDC, size.cx, size.cy);
    HBITMAP OriBitmap = (HBITMAP)SelectObject(overlayDC, overlayBitmap);

    Graphics g(overlayDC);

    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    g.SetPixelOffsetMode(PixelOffsetModeHalf);

    g.DrawImage(image_.get(), 0, 0, size.cx, size.cy);

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 200, AC_SRC_ALPHA };

    auto ret = UpdateLayeredWindow(hwnd(), NULL, &pos, &size, overlayDC,
        &zero, 0, &bf, ULW_ALPHA);
    _ASSERT(ret);

    SelectObject(overlayDC, OriBitmap);
    DeleteObject(overlayBitmap);
    DeleteDC(overlayDC);
}

void ShadowForm::move(POINT &pt)
{
    setup.position = pt;
    SetWindowPos(hwnd(), 0, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
}

void ShadowForm::zoom(POINT& pt, SIZE& size)
{
    width_ = round(((float)size.cx / setup.size.cx) * width_);
    height_ = round(((float)size.cy / setup.size.cy) * height_);

    setup.position = pt;
    setup.size = size;

    DrawShadowUI();
}
