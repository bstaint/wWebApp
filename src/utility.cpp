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
