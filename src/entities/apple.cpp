#include "apple.h"

Apple::Apple(int x, int y, int radius)
    : Apple(Point(x, y), radius)
{
}

Apple::Apple(Point point, int radius)
    : m_position(point)
    , m_radius(radius)
{
}
