#pragma once

#include "geometrytypes.h"

class SnakeSegment
{
public:
    SnakeSegment(int x, int y, int radius);

    SnakeSegment(Point point, int radius);

    void moveOn(Vector vector);

    void moveTo(Point point);

    [[nodiscard]] Point position() const { return m_point; }

    [[nodiscard]] int radius() const { return m_radius; }

private:
    Point m_point;
    int m_radius;
};
