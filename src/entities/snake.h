#pragma once

#include "snakesegment.h"

#include <vector>

class Snake
{
public:
    Snake(Point point, int headRadius, int bodyRadius);

    Snake(int x, int y, int headRadius, int bodyRadius);

    void moveOn(Vector vector);

    void appendSegment();

    [[nodiscard]] bool contains(Point point) const;

    [[nodiscard]] Point position() const { return m_head.position(); }

    [[nodiscard]] int headRadius() const { return m_head.radius(); }

    [[nodiscard]] int bodyRadius() const { return m_bodyRadius; }

private:
    SnakeSegment m_head;
    std::vector<SnakeSegment> m_body;

    int m_bodyRadius;
};
