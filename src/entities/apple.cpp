#include "apple.h"

Apple::Apple(int x, int y, int sideLength)
    : Apple(Point(x, y), sideLength)
{
}

Apple::Apple(Point point, int sideLength)
    : m_position(point)
    , m_sideLength(sideLength)
{
}
