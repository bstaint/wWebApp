#ifndef UTILITY_H
#define UTILITY_H

#include "Windows.h"

SIZE operator+(SIZE const & L, int const & R) {
    return {L.cx + R, L.cy + R};
}

SIZE operator-(SIZE const & L, int const & R) {
    return {L.cx - R, L.cy - R};
}

#endif // UTILITY_H
