#ifndef GDIPLUSHELPER_H
#define GDIPLUSHELPER_H

#include <gdiplus.h>

using namespace Gdiplus;

class GdiPlusHelper
{
    ULONG_PTR	m_gdiplusToken;

public:
    GdiPlusHelper()
    {
        GdiplusStartupInput input;
        GdiplusStartup(&m_gdiplusToken, &input, NULL);
    }

    ~GdiPlusHelper()
    {
         GdiplusShutdown(m_gdiplusToken);
    }
};

#endif // GDIPLUSHELPER_H
