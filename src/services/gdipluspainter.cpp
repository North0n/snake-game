#include "gdipluspainter.h"

#include <algorithm>

GdiPlusPainter::GdiPlusPainter(HDC hdc)
    : m_hdc(hdc)
    , m_graphics(hdc)
{
}

void GdiPlusPainter::draw(const Snake& snake)
{
    // Draw head
    m_graphics.DrawRectangle(&m_snakePen, snake.position().x, snake.position().y, snake.headSideLength(), snake.headSideLength());

    // Draw body
    std::for_each(snake.body().begin(), snake.body().end(), [this](const auto& segment) {
        m_graphics.DrawRectangle(&m_snakePen, segment.position().x, segment.position().y, segment.sideLength(), segment.sideLength());
    });
}

void GdiPlusPainter::draw(const Apple& apple)
{
    m_graphics.DrawRectangle(&m_applePen, apple.position().x, apple.position().y, apple.sideLength(), apple.sideLength());
}
