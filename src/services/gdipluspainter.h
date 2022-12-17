#pragma once

#include "entities/apple.h"
#include "entities/gametypes.h"
#include "entities/snake.h"

#include <Windows.h>
#include <gdiplus.h>
#include <list>
#include <optional>
#pragma comment(lib, "Gdiplus.lib")

class ImageContainer;

class GdiPlusPainter
{
public:
    GdiPlusPainter(Gdiplus::Graphics& graphics);
    ~GdiPlusPainter();

    void draw(const Snake& snake, const Vector& direction);
    void draw(const Apple& apple);
    void draw(const std::vector<Point>& obstacles);
    void draw(int score);
    void draw(const std::list<Record>& records,
              const std::optional<Difficulty>& filterDifficulty,
              const std::optional<int>& filterMapIndex,
              const std::optional<std::wstring>& filterName,
              int maxRecordsCount);
    void drawPause(int width, int height);

private:
    static void drawRoundRect(Gdiplus::Graphics* pGraphics, Gdiplus::Rect r,  Gdiplus::Color color, int radius, int width);
    static void getRoundRectPath(Gdiplus::GraphicsPath *pPath, Gdiplus::Rect r, int dia);

    Gdiplus::Graphics& m_graphics;

    Gdiplus::Pen m_snakePen{Gdiplus::Color::Black, 2.0};
    Gdiplus::Pen m_applePen{Gdiplus::Color::YellowGreen};
    Gdiplus::Pen m_obstaclePen{Gdiplus::Color::Gray};

    Gdiplus::SolidBrush m_snakeBrush{Gdiplus::Color::Red};
    Gdiplus::SolidBrush m_appleBrush{Gdiplus::Color::YellowGreen};
    Gdiplus::SolidBrush m_obstacleBrush{Gdiplus::Color::Gray};

    Gdiplus::SolidBrush m_textBrush{Gdiplus::Color::Black};

    ImageContainer* m_appleImage;
    ImageContainer* m_headImage;
};
