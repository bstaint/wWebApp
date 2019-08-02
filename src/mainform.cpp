#include "mainform.h"
#include "utility.h"
#include <sstream>

using namespace wl;

// 边框差额
static const int kBorderPadding = 1;
// 最小窗口大小
static const int kWindowWidth = 600;
// 最大窗口大小
static const int kWindowHeight = 400;

MainForm::MainForm()
{
    setup.wndClassEx.lpszClassName = L"MainFromClass";
    setup.wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    // WS_THICKFRAME 有默认WM_SETCURSOR处理操作
    setup.style |= (ws)WS_THICKFRAME;
    setup.size = {kWindowWidth, kWindowHeight};
    setup.position = {5, 5};

    on_message(WM_CREATE, [&](params){
        shadow_.bindWindow(hwnd());
        shadow_.create(this, 0, setup.position, setup.size);
        webview_.create(this, 0, {0, 0}, setup.size - kBorderPadding);

        return 0;
    });

    // 窗口大小相关消息处理
    WinSizeEventHandler();

    // NC相关消息处理
    NcEventHandler();
}

bool MainForm::isWindowMove(const POINT &pt)
{
    bool bChanged = setup.position.x != pt.x || setup.position.y != pt.y;
    if(bChanged)
    {
        setup.position = pt;
        return true;
    }

    return false;
}

bool MainForm::isWindowZoom(const SIZE &size)
{
    bool bChanged = setup.size.cx != size.cx || setup.size.cy != size.cy;
    if(bChanged)
    {
        setup.size = size;
        return true;
    }

    return false;
}

void MainForm::NcEventHandler()
{
    on_message({WM_NCACTIVATE, WM_NCCALCSIZE}, [&](params){
        return 0;
    });

    on_message(WM_NCHITTEST, [&](wm::nchittest p){

        RECT rc;
        POINT mouse = { p.pos().x, p.pos().y };

        GetClientRect(hwnd(), &rc);
        ScreenToClient(hwnd(), &mouse);

        int ww = rc.right - rc.left;
        int wh = rc.bottom - rc.top;

        int frame_size = GetSystemMetrics(SM_CXFRAME) +
                         GetSystemMetrics(SM_CXPADDEDBORDER);
        /* The diagonal size handles are wider than the frame */
        int diagonal_width = frame_size * 2 + GetSystemMetrics(SM_CXBORDER);

        if (mouse.y < frame_size) {
            if (mouse.x < diagonal_width)
                return HTTOPLEFT;
            if (mouse.x >= ww - diagonal_width)
                return HTTOPRIGHT;
            return HTTOP;
        }

        if (mouse.y >= wh - frame_size) {
            if (mouse.x < diagonal_width)
                return HTBOTTOMLEFT;
            if (mouse.x >= ww - diagonal_width)
                return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }

        if (mouse.x < frame_size)
            return HTLEFT;
        if (mouse.x >= ww - frame_size)
            return HTRIGHT;

        return HTCLIENT;
    });
}

void MainForm::WinSizeEventHandler()
{
    // The WM_SIZE and WM_MOVE messages are not sent if an application handles the WM_WINDOWPOSCHANGED message
    on_message(WM_WINDOWPOSCHANGED, [&](wm::windowposchanged p){

        POINT pt = {p.windowpos().x, p.windowpos().y};
        SIZE size = {p.windowpos().cx, p.windowpos().cy};

        if(isWindowMove(pt))
        {
            shadow_.setup.position = pt;
            SetWindowPos(shadow_.hwnd(), 0, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
        }

        if(isWindowZoom(size))
        {
            shadow_.setup.position = pt;
            shadow_.setup.size = size;
            shadow_.DrawShadowUI();
            webview_.resize(size - kBorderPadding);
        }

        return 0;
    });

    on_message(WM_GETMINMAXINFO, [&](wm::getminmaxinfo p){

        p.minmaxinfo().ptMinTrackSize = {kWindowWidth, kWindowHeight};
        p.minmaxinfo().ptMaxPosition = {0, 0};

        RECT rc;
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
        p.minmaxinfo().ptMaxSize.x = p.minmaxinfo().ptMaxTrackSize.x = rc.right - rc.left;
        p.minmaxinfo().ptMaxSize.y = p.minmaxinfo().ptMaxTrackSize.y = rc.bottom - rc.top;

        return 0;
    });
}
