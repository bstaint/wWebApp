#include "blinkwidget.h"
#include "utility.h"
#include "callback.h"

using namespace wl;

std::once_flag blinkWidget::s_ready_flag;

blinkWidget::blinkWidget() :
    webview_(wkeCreateWebView()),
    zip_("../res/ui.zip"),
    cursor_type_(WkeCursorInfoPointer)
{
    setup.wndClassEx.lpszClassName = L"MINIBLINK_WIDGET";
    setup.style |= ws::CHILD | ws::VISIBLE;
#ifdef _DEBUG
//    setup.exStyle |= wsx::CLIENTEDGE;
#endif

    OnWkeInit();

    on_message(WM_CREATE, [&](params){
        RECT rc = { 0 };
        ::GetClientRect(hwnd(), &rc);

        wkeResize(webview_, rc.right, rc.bottom);
//        wkeMoveToCenter(webview_);
//        wkeLoadURL(webview_, u8"file:///../res/template.html");
//        wkeLoadURL(webview_, u8"http://www.baidu.com");
        wkeLoadURL(webview_, u8"mb://ui/template.html");

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

        // 将WM_NCHITTEST抛给父窗口处理
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
    wkeJsBindFunction("url", urlCallback, this, 1);

    // 打开DevTools
//    wkeSetDebugConfig(webview_, "showDevTools", u8"file:///E:/3rdParty/extras/miniblink/front_end/inspector.html");

    wkeOnLoadUrlBegin(webview_, [](wkeWebView, void* param, const char *url, void *job){
        // 判断URL是否为mb://开头
        if(strncmp(url, APP_SAFE_URL, strlen(APP_SAFE_URL)) == 0)
        {
            blinkWidget *ths = static_cast<blinkWidget *>(param);
            ByteVector& bytes = ths->zip_.getBytes(url + strlen(APP_SAFE_URL));

            wkeNetSetData(job, bytes.data(), bytes.size());
            return true;
        }
        return false;
    }, this);

    wkeOnPaintUpdated(webview_, [](wkeWebView, void* param, const HDC, int, int, int, int){
            auto ptr = (blinkWidget *)param;
            InvalidateRect(ptr->hwnd(), NULL, false);
            UpdateWindow(ptr->hwnd());
    }, this);

    wkeOnDocumentReady(webview_, [](wkeWebView, void* param){
        std::call_once(s_ready_flag, [param](){
            HWND hwnd = ::GetParent(static_cast<blinkWidget *>(param)->hwnd());
            PostMessage(hwnd, CM_READY_SHOW, 0, 0);
        });
    }, this);
}

void blinkWidget::KeyEventHandler()
{
    // 无法接收键盘消息需要处理WM_GETDLGCODE
    on_message(WM_GETDLGCODE, [&](params){
        return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
    });

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
        } else if(p.message == WM_MOUSEMOVE)
        {
            cursor_type_ = wkeGetCursorInfoType(webview_);
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

    on_message(WM_SETCURSOR, [&](wm::setcursor){
//        TODO: 等待完善，参考demo_src中代码
        HCURSOR hCur = NULL;
        switch(cursor_type_)
        {
        case WkeCursorInfoPointer:
            hCur = ::LoadCursor(NULL, IDC_ARROW);
            break;
        case WkeCursorInfoHand:
            hCur = ::LoadCursor(NULL, IDC_HAND);
            break;
        default:
            hCur = ::LoadCursor(NULL, IDC_ARROW);
        }

        if(hCur)
            ::SetCursor(hCur);

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

void blinkWidget::resize(const SIZE &size)
{
    setup.size = size;
    wkeResize(webview_, size.cx, size.cy);
    MoveWindow(hwnd(), setup.position.x, setup.position.y, size.cx, size.cy, TRUE);
}
