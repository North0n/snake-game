#include "snakesegment.h"

SnakeSegment::SnakeSegment(int x, int y, int radius)
    : SnakeSegment(Point(x, y), radius)
{
}

SnakeSegment::SnakeSegment(Point point, int radius)
    : m_point(point)
    , m_radius(radius)
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

