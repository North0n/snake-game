#include "heart.h"

Heart::Heart(int x, int y, int sideLength)
    : Heart(Point(x, y), sideLength)
{
}

Heart::Heart(Point point, int sideLength)
    : m_position(point)
    , m_sideLength(sideLength)
{
}
