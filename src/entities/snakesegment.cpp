#include "snakesegment.h"

SnakeSegment::SnakeSegment(int x, int y, int sideLength)
    : SnakeSegment(Point(x, y), sideLength)
{
}

SnakeSegment::SnakeSegment(Point point, int sideLength)
    : m_point(point)
    , m_sideLength(sideLength)
{
}

void SnakeSegment::moveOn(Vector vector)
{
    m_point.x += vector.x;
    m_point.y += vector.y;
}

void SnakeSegment::moveTo(Point point)
{
    m_point = point;
}

