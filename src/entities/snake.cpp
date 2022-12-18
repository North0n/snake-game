#include "snake.h"

#include <algorithm>

Snake::Snake(Point point, int headSideLength, int bodySideLength)
    : m_head(point, headSideLength)
    , m_bodySideLength(bodySideLength)
{
}

Snake::Snake(int x, int y, int headSideLength, int bodySideLength)
    : Snake(Point(x, y), headSideLength, bodySideLength)
{
}

void Snake::moveOn(Vector vector, int width, int height)
{
    auto headPosition = m_head.position();
    m_head.moveOn(vector, width, height);

    for (auto& segment : m_body) {
        auto segmentPosition = segment.position();
        segment.moveTo(headPosition);
        headPosition = segmentPosition;
    }
}

void Snake::appendSegment()
{
    m_body.emplace_back(m_head.position(), m_bodySideLength);
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
