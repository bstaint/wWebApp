#ifndef UTILITY_H
#define UTILITY_H

#include "Windows.h"

#define CM_READY_SHOW WM_USER + 100

SIZE operator+(SIZE const & L, int const & R);

SIZE operator-(SIZE const & L, int const & R);

POINT operator+(POINT const & L, int const & R);

POINT operator-(POINT const & L, int const & R);

#endif // UTILITY_H
