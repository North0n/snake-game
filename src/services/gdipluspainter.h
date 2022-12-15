#pragma once

#include "entities/apple.h"
#include "entities/snake.h"

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

class GdiPlusPainter
{
public:
    GdiPlusPainter(HDC hdc);

    void draw(const Snake& snake);
    void draw(const Apple& apple);

private:
    HDC m_hdc{};
    Gdiplus::Graphics m_graphics;
    Gdiplus::Pen m_snakePen{Gdiplus::Color::Red};
    Gdiplus::Pen m_applePen{Gdiplus::Color::YellowGreen};
};
