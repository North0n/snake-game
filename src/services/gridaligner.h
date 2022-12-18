#pragma once

#include "entities/geometrytypes.h"

class GridAligner
{
public:
    explicit GridAligner(int cellSize);

    [[nodiscard]] int toCellCoords(int value) const;
    [[nodiscard]] Point toCellCoords(const Point& point) const;
    [[nodiscard]] Vector toCellCoords(const Vector& vector) const;

private:
    int m_cellSize;
};
