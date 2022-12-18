#pragma once

#include "geometrytypes.h"

class SnakeSegment
{
public:
    SnakeSegment(int x, int y, int sideLength);

    SnakeSegment(Point point, int sideLength);

    void moveOn(Vector vector, int width, int height);

    void moveTo(Point point);

    [[nodiscard]] Point position() const { return m_point; }

    [[nodiscard]] int sideLength() const { return m_sideLength; }

private:
    Point m_point;
    int m_sideLength;
};
