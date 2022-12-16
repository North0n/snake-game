#pragma once

#include "entities/apple.h"
#include "entities/snake.h"

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

class GdiPlusPainter
{
public:
    GdiPlusPainter(Gdiplus::Graphics& graphics);

    void draw(const Snake& snake);
    void draw(const Apple& apple);
    void draw(int score);

private:
    Gdiplus::Graphics& m_graphics;

    Gdiplus::Pen m_snakePen{Gdiplus::Color::Black, 2.0};
    Gdiplus::Pen m_applePen{Gdiplus::Color::YellowGreen};

    Gdiplus::SolidBrush m_snakeBrush{Gdiplus::Color::Red};
    Gdiplus::SolidBrush m_appleBrush{Gdiplus::Color::YellowGreen};
    Gdiplus::SolidBrush m_textBrush{Gdiplus::Color::Black};
};
