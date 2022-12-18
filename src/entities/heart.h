#pragma once

#include "geometrytypes.h"

class Heart
{
public:
    Heart(int x, int y, int sideLength);

    Heart(Point point, int sideLength);

    [[nodiscard]] Point position() const { return m_position; }

    [[nodiscard]] int sideLength() const { return m_sideLength; }

private:
    Point m_position;
    int m_sideLength;
};
