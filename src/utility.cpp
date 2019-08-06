#include "utility.h"

SIZE operator+(SIZE const & L, int const & R) {
    return {L.cx + R, L.cy + R};
}

SIZE operator-(SIZE const & L, int const & R) {
    return {L.cx - R, L.cy - R};
}

POINT operator+(POINT const & L, int const & R) {
    return {L.x + R, L.y + R};
}

POINT operator-(POINT const & L, int const & R) {
    return {L.x - R, L.y - R};
}

POINT getWndCenter(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int x = (rc.right - rc.left) / 2;
    int y = (rc.bottom - rc.top) / 2;

    return {x, y};
}
