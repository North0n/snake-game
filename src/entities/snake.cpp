#include "snake.h"

#include <algorithm>

Snake::Snake(Point point, int headRadius, int bodyRadius)
    : m_head(point, headRadius)
    , m_bodyRadius(bodyRadius)
{
}

Snake::Snake(int x, int y, int headRadius, int bodyRadius)
    : Snake(Point(x, y), headRadius, bodyRadius)
{
}

void Snake::moveOn(Vector vector)
{
    auto headPosition = m_head.position();
    m_head.moveOn(vector);

    for (auto& segment : m_body) {
        auto segmentPosition = segment.position();
        segment.moveTo(headPosition);
        headPosition = segmentPosition;
    }
}

void Snake::appendSegment()
{
    m_body.emplace_back(Point{0, 0}, m_bodyRadius);
}

bool Snake::contains(Point point) const
{
    if (m_head.position() == point) {
        return true;
    }

    return std::any_of(m_body.begin(), m_body.end(), [&point](const auto& segment) {
        return segment.position() == point;
    });
}
