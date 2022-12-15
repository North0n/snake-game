#include "gdipluspainter.h"

#include <algorithm>

using namespace Gdiplus;

GdiPlusPainter::GdiPlusPainter(Graphics& graphics)
    : m_graphics(graphics)
{
}

void GdiPlusPainter::draw(const Snake& snake)
{
    // Draw head
    m_graphics.DrawRectangle(&m_snakePen, snake.position().x, snake.position().y, snake.headSideLength(), snake.headSideLength());
    m_graphics.FillRectangle(&m_snakeBrush, snake.position().x, snake.position().y, snake.headSideLength(), snake.headSideLength());

    // Draw body
    std::for_each(snake.body().begin(), snake.body().end(), [this](const auto& segment) {
        m_graphics.DrawRectangle(&m_snakePen, segment.position().x, segment.position().y, segment.sideLength(), segment.sideLength());
        m_graphics.FillRectangle(&m_snakeBrush, segment.position().x, segment.position().y, segment.sideLength(), segment.sideLength());
    });
}

void GdiPlusPainter::draw(const Apple& apple)
{
    m_graphics.DrawRectangle(&m_applePen, apple.position().x, apple.position().y, apple.sideLength(), apple.sideLength());
    m_graphics.FillRectangle(&m_appleBrush, apple.position().x, apple.position().y, apple.sideLength(), apple.sideLength());
}
