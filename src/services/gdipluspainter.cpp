#include "gdipluspainter.h"

#include <algorithm>
#include <string>
#include <sstream>

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

void GdiPlusPainter::draw(int score)
{
    std::wstringstream ss;
    ss << L"Ñ÷¸ò: " << score;
    auto str = ss.str();
    auto font = Font(L"Arial Rounded MT", 20);
    m_graphics.DrawString(str.c_str(), str.length(), &font, PointF(0, 0), &m_textBrush);
}

void GdiPlusPainter::draw(const std::vector<::Point>& obstacles)
{
    std::for_each(obstacles.begin(), obstacles.end(), [this](const auto& obstacle) {
        m_graphics.DrawRectangle(&m_obstaclePen, obstacle.x, obstacle.y, 40, 40);
        m_graphics.FillRectangle(&m_obstacleBrush, obstacle.x, obstacle.y, 40, 40);
    });
}
