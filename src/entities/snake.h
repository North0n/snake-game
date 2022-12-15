#pragma once

#include "snakesegment.h"

#include <vector>

class Snake
{
public:
    Snake(Point point, int headSideLength, int bodySideLength);

    Snake(int x, int y, int headSideLength, int bodySideLength);

    void moveOn(Vector vector);

    void appendSegment();

    [[nodiscard]] bool contains(Point point) const;

    [[nodiscard]] Point position() const { return m_head.position(); }

    [[nodiscard]] const std::vector<SnakeSegment>& body() const { return m_body; }

    [[nodiscard]] int headSideLength() const { return m_head.sideLength(); }

    [[nodiscard]] int bodySideLength() const { return m_bodySideLength; }

private:
    SnakeSegment m_head;
    std::vector<SnakeSegment> m_body;

    int m_bodySideLength;
};
