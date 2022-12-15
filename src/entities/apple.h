#pragma once

#include "geometrytypes.h"

class Apple
{
public:
    Apple(int x, int y, int radius);

    Apple(Point point, int radius);

private:
    Point m_position;
    int m_radius;
};
