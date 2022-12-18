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

void SnakeSegment::moveOn(Vector vector, int width, int height)
{
    m_point.x = (m_point.x + vector.x + width) % width;
    m_point.y = (m_point.y + vector.y + height) % height;
}

void SnakeSegment::moveTo(Point point)
{
    m_point = point;
}

