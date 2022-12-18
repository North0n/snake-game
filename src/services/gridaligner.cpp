#include "gridaligner.h"

GridAligner::GridAligner(int cellSize)
    : m_cellSize(cellSize)
{
}

int GridAligner::toCellCoords(int value) const
{
    return static_cast<int>(round(value / static_cast<double>(m_cellSize)) * m_cellSize);
}

Point GridAligner::toCellCoords(const Point& point) const
{
    return {toCellCoords(point.x), toCellCoords(point.y)};
}

Vector GridAligner::toCellCoords(const Vector& vector) const
{
    return {toCellCoords(vector.x), toCellCoords(vector.y)};
}
