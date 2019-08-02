#include "blinkwidget.h"
#include <sstream>
#include <cassert>
#include <fstream>

using namespace wl;

#define LOCAL_URI ".."
#define APP_SAFE_URL "mb://"
#define NOT_FOUND_STR "<h2>Not Found</h2>"

jsValue exeCallback(jsExecState es, void *param)
{
    assert(param != nullptr);
    HWND hwnd = ::GetParent(static_cast<blinkWidget *>(param)->hwnd());

    ReleaseCapture();
    SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);

    return jsUndefined();
}

blinkWidget::blinkWidget() :
    webview_(wkeCreateWebView())
{
    setup.wndClassEx.lpszClassName = L"MINIBLINK_WIDGET";
    setup.wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    setup.style |= ws::CHILD | ws::VISIBLE;
#ifdef _DEBUG
//    setup.exStyle |= wsx::CLIENTEDGE;
#endif

    OnWkeInit();

    on_message(WM_CREATE, [&](params){
        RECT rc = { 0 };
        ::GetClientRect(hwnd(), &rc);

        wkeResize(webview_, rc.right, rc.bottom);
        wkeMoveToCenter(webview_);
        wkeLoadURL(webview_, u8"file:///../res/template.html");

        return 0;
    });
    
     on_message(WM_NCHITTEST, [&](wm::nchittest p){
        RECT rc;
        POINT mouse = { p.pos().x, p.pos().y };

        GetClientRect(GetParent(hwnd()), &rc);
        ScreenToClient(GetParent(hwnd()), &mouse);

        int wh = rc.bottom - rc.top;

        int frame_size = GetSystemMetrics(SM_CXFRAME) +
                         GetSystemMetrics(SM_CXPADDEDBORDER);

        if (mouse.y >= wh - frame_size) {
            return HTTRANSPARENT;
        }

        return HTCLIENT;
    });

    // 鼠标消息处理
    MouseEventHandler();

    // 绘制消息处理
    PaintEventHandler();

    // 键盘消息处理
    KeyEventHandler();

    // 焦点消息处理
    FocusEventHandler();
}

void blinkWidget::OnWkeInit()
{
    wkeSetHandle(webview_, hwnd());
    wkeSetNavigationToNewWindowEnable(webview_, false);

    wkeJsBindFunction("exe", exeCallback, this, 1);

    wkeOnPaintUpdated(webview_, [](wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy){
            auto ptr = (blinkWidget *)param;
            InvalidateRect(ptr->hwnd(), NULL, false);
            UpdateWindow(ptr->hwnd());
    }, this);

    wkeOnDocumentReady(webview_, [](wkeWebView webView, void* param){
        wkeShowWindow(webView, true);
    }, this);

    /*
    wkeOnLoadUrlBegin(m_view, [](wkeWebView webView, void* param, const char *url, void *job){

        std::vector<char> data;

        if(strncmp(url, APP_SAFE_URL, strlen(APP_SAFE_URL)) == 0)
        {
            blinkWidget *ths = static_cast<blinkWidget *>(param);
            std::stringstream sstream;
            sstream << LOCAL_URI << url + (strlen(APP_SAFE_URL) - 1);
            OutputDebugStringA(sstream.str().c_str());

            ths->readFile(sstream.str(), data);
            wkeNetSetData(job, data.data(), data.size());

            return true;
        }

        return false;

    }, this);
    */
}

void blinkWidget::KeyEventHandler()
{
    on_message({WM_KEYDOWN, WM_KEYUP, WM_CHAR}, [&](wm::keydown p){

        unsigned int flags = 0;
        if (p.key_previously_down())
            flags |= WKE_REPEAT;
        if (p.is_extended_key())
            flags |= WKE_EXTENDED;

        switch (p.message) {
            case WM_KEYDOWN:
                wkeFireKeyDownEvent(webview_, p.vkey_code(), flags, false);
                break;
            case WM_KEYUP:
                wkeFireKeyUpEvent(webview_, p.vkey_code(), flags, false);
                break;
            case WM_CHAR:
                wkeFireKeyPressEvent(webview_, p.vkey_code(), flags, false);
                break;
            default:
                break;
        }

        return 0;
    });

    // 无法接收键盘消息需要处理WM_GETDLGCODE
    on_message(WM_GETDLGCODE, [&](params){
        return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
    });
}

void blinkWidget::PaintEventHandler()
{
    on_message(WM_PAINT, [&](params){
        gdi::dc_painter painter(this);
        BitBlt(painter.hdc(), 0, 0, setup.size.cx, setup.size.cy, wkeGetViewDC(webview_), 0, 0, SRCCOPY);
        return 0;
    });

    on_message(WM_ERASEBKGND, [&](params){ return 0; });
}

void blinkWidget::MouseEventHandler()
{
    on_message({WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MBUTTONDOWN,
                WM_LBUTTONUP, WM_MBUTTONUP, WM_RBUTTONUP,
                WM_LBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_RBUTTONDBLCLK,
                WM_MOUSEMOVE}, [&](wm::lbuttondown p)
    {
        if (p.message == WM_LBUTTONDOWN
                || p.message == WM_MBUTTONDOWN
                || p.message == WM_RBUTTONDOWN) {
            SetFocus(hwnd());
            SetCapture(hwnd());
        } else if (p.message == WM_LBUTTONUP
                   || p.message == WM_MBUTTONUP
                   || p.message == WM_RBUTTONUP) {
            ReleaseCapture();
        }

        unsigned int flags = 0;

        if (p.has_ctrl())
            flags |= WKE_CONTROL;
        if (p.has_shift())
            flags |= WKE_SHIFT;

        if (p.has_left_btn())
            flags |= WKE_LBUTTON;
        if (p.has_middle_btn())
            flags |= WKE_MBUTTON;
        if (p.has_right_btn())
            flags |= WKE_RBUTTON;

        wkeFireMouseEvent(webview_, p.message, p.pos().x, p.pos().y, flags);

        return 0;
    });
}

void blinkWidget::FocusEventHandler()
{
    on_message(WM_SETFOCUS, [&](params){
        wkeSetFocus(webview_);
        return 0;
    });

    on_message(WM_KILLFOCUS, [&](params){
        wkeKillFocus(webview_);
        return 0;
    });
}

void blinkWidget::readFile(const std::string &path, std::vector<char> &data)
{
    std::fstream in(path);
    if(in.is_open())
    {
        in.seekg(0, std::ios_base::end);
        std::streampos size = in.tellg();
        data.resize(size);

        in.seekg(0, std::ios_base::beg);
        in.read(&data[0], size);
    }
}

void blinkWidget::resize(const SIZE &size)
{
    setup.size = size;
    wkeResize(webview_, size.cx, size.cy);
    MoveWindow(hwnd(), setup.position.x, setup.position.y, size.cx, size.cy, TRUE);
}
