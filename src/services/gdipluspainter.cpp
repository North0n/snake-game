#include "gdipluspainter.h"

#include "entities/imagecontainer.h"
#include "entities/heart.h"

#include <algorithm>
#include <string>

using namespace Gdiplus;

GdiPlusPainter::GdiPlusPainter(Graphics& graphics)
    : m_graphics(graphics)
    , m_appleImage(new ImageContainer)
    , m_headImage(new ImageContainer)
    , m_heartImage(new ImageContainer)
{
    m_appleImage->load(L"Apple", L"Image");
    m_headImage->load(L"Head", L"Image");
    m_heartImage->load(L"Heart", L"Image");
}

GdiPlusPainter::~GdiPlusPainter()
{
    delete m_appleImage;
    delete m_headImage;
    delete m_heartImage;
}

void GdiPlusPainter::draw(const Snake& snake, const Vector& direction, bool isVulnerable)
{
    // Draw head
    Matrix	matrix;
    REAL angle = 0;
    if (direction == Vector{1, 0}) {
        angle = 90;
    } else if (direction == Vector{0, 1}) {
        angle = 180;
    } else if (direction == Vector{-1, 0}) {
        angle = 270;
    }
    matrix.RotateAt(angle, PointF(snake.position().x + snake.headSideLength() / 2.0f, snake.position().y + snake.headSideLength() / 2.0f));
    m_graphics.SetTransform(&matrix);
    m_graphics.DrawRectangle(&m_snakePen, snake.position().x, snake.position().y, snake.headSideLength(), snake.headSideLength());
    m_graphics.DrawImage(*m_headImage, snake.position().x, snake.position().y, snake.headSideLength(), snake.headSideLength());
    m_graphics.ResetTransform();

    // Draw body
    if (isVulnerable) {
        m_snakeBrush.SetColor(Color::Orange);
    } else {
        m_snakeBrush.SetColor(Color::Red);
    }
    std::for_each(snake.body().begin(), snake.body().end(), [this](const auto& segment) {
        m_graphics.DrawRectangle(&m_snakePen, segment.position().x, segment.position().y, segment.sideLength(), segment.sideLength());
        m_graphics.FillRectangle(&m_snakeBrush, segment.position().x, segment.position().y, segment.sideLength(), segment.sideLength());
    });
}

void GdiPlusPainter::draw(const Apple& apple)
{
    m_graphics.DrawImage(*m_appleImage, apple.position().x, apple.position().y, apple.sideLength(), apple.sideLength());
}

void GdiPlusPainter::draw(const Heart& heart)
{
    m_graphics.DrawImage(*m_heartImage, heart.position().x, heart.position().y, heart.sideLength(), heart.sideLength());
}

void GdiPlusPainter::drawScore(int score)
{
    m_graphics.DrawString((L"Счёт: " + std::to_wstring(score)).c_str(), -1, &m_font, PointF(0, 0), &m_textBrush);
}

void GdiPlusPainter::drawLives(int livesCount)
{
    m_graphics.DrawString((L"Жизни: " + std::to_wstring(livesCount)).c_str(), -1, &m_font, PointF(0, 30), &m_textBrush);
}

void GdiPlusPainter::draw(const std::vector<::Point>& obstacles)
{
    std::for_each(obstacles.begin(), obstacles.end(), [this](const auto& obstacle) {
        m_graphics.DrawRectangle(&m_obstaclePen, obstacle.x, obstacle.y, 40, 40);
        m_graphics.FillRectangle(&m_obstacleBrush, obstacle.x, obstacle.y, 40, 40);
    });
}

void GdiPlusPainter::draw(const std::list<Record>& records,
                          const std::optional<Difficulty>& filterDifficulty,
                          const std::optional<int>& filterMapIndex,
                          const std::optional<std::wstring>& filterName,
                          int maxRecordsCount)
{
    auto font = new Font(L"Arial Rounded MT", 20);
    SolidBrush brush(Color::Black);

    if (maxRecordsCount == -1)
        maxRecordsCount = records.size();

    // Draw header
    SolidBrush brush2(Color::LightGray);
    m_graphics.FillRectangle(&brush2, 250, 25, 100, 32);
    m_graphics.DrawString(L"Номер", -1, font, PointF(250, 25), &brush);
    m_graphics.FillRectangle(&brush2, 550, 25, 100, 32);
    m_graphics.DrawString(L"Счёт", -1, font, PointF(550, 25), &brush);

    // Draw records
    int i = 0;
    for (auto it = records.cbegin(); it != records.cend() && i < maxRecordsCount; ++it) {
        if ((!filterDifficulty || it->difficulty == *filterDifficulty) &&
            (!filterMapIndex || it->mapIndex == *filterMapIndex) &&
            (!filterName || it->name == *filterName)) {
            m_graphics.DrawString(std::to_wstring(i + 1).c_str(), -1, font, PointF(250, 75 + i * 50), &brush);
            m_graphics.DrawString(it->name.c_str(), -1, font, PointF(350, 75 + i * 50), &brush);
            m_graphics.DrawString(std::to_wstring(it->score).c_str(), -1, font, PointF(550, 75 + i * 50), &brush);
            std::wstring difficulty;
            switch (it->difficulty) {
            case Difficulty::Easy:
                difficulty = L"Лёгкая";
                break;
            case Difficulty::Medium:
                difficulty = L"Средняя";
                break;
            case Difficulty::Hard:
                difficulty = L"Сложная";
                break;
            }
            m_graphics.DrawString(difficulty.c_str(), -1, font, PointF(650, 75 + i * 50), &brush);
            m_graphics.DrawString((std::wstring(L"Карта ") + std::to_wstring(it->mapIndex + 1)).c_str(), -1, font, PointF(800, 75 + i * 50), &brush);
            ++i;
        }
    }

    // Draw empty records
    for (int j = i; j < maxRecordsCount; ++j) {
        m_graphics.DrawString(std::to_wstring(j + 1).c_str(), -1, font, PointF(250, 75 + j * 50), &brush);
        m_graphics.DrawString(L"-", -1, font, PointF(350, 75 + j * 50), &brush);
        m_graphics.DrawString(L"-", -1, font, PointF(550, 75 + j * 50), &brush);
        m_graphics.DrawString(L"-", -1, font, PointF(650, 75 + j * 50), &brush);
        m_graphics.DrawString(L"-", -1, font, PointF(800, 75 + j * 50), &brush);
    }

    // Draw table
    Pen pen(Color::Black);
    m_graphics.DrawLine(&pen, 350, 25, 350, 475);
    m_graphics.DrawLine(&pen, 550, 25, 550, 475);
    m_graphics.DrawLine(&pen, 650, 25, 650, 475);
    m_graphics.DrawLine(&pen, 800, 25, 800, 475);

    m_graphics.DrawLine(&pen, 250, 57, 950, 57);

    drawRoundRect(&m_graphics, Rect{240, 15, 720, 470}, Gdiplus::Color(41, 245, 92), 10, 10);
}

void GdiPlusPainter::getRoundRectPath(GraphicsPath* pPath, Rect r, int dia)
{
    // diameter can't exceed width or height
    if (dia > r.Width)
        dia = r.Width;
    if (dia > r.Height)
        dia = r.Height;

    // define a corner
    Rect Corner(r.X, r.Y, dia, dia);

    // begin path
    pPath->Reset();

    // top left
    pPath->AddArc(Corner, 180, 90);

    // tweak needed for radius of 10 (dia of 20)
    if (dia == 20) {
        Corner.Width += 1;
        Corner.Height += 1;
        r.Width -= 1;
        r.Height -= 1;
    }

    // top right
    Corner.X += (r.Width - dia - 1);
    pPath->AddArc(Corner, 270, 90);

    // bottom right
    Corner.Y += (r.Height - dia - 1);
    pPath->AddArc(Corner, 0, 90);

    // bottom left
    Corner.X -= (r.Width - dia - 1);
    pPath->AddArc(Corner, 90, 90);

    // end path
    pPath->CloseFigure();
}

void GdiPlusPainter::drawRoundRect(Graphics* pGraphics, Rect r, Color color, int radius, int width)
{
    int dia = 2 * radius;

    // set to pixel mode
    int oldPageUnit = pGraphics->SetPageUnit(UnitPixel);

    // define the pen
    Pen pen(color, 1);
    pen.SetAlignment(PenAlignmentCenter);

    // get the corner path
    GraphicsPath path;

    // get path
    getRoundRectPath(&path, r, dia);

    // draw the round rect
    pGraphics->DrawPath(&pen, &path);

    // if width > 1
    for (int i = 1; i < width; i++) {
        // left stroke
        r.Inflate(-1, 0);
        // get the path
        getRoundRectPath(&path, r, dia);

        // draw the round rect
        pGraphics->DrawPath(&pen, &path);

        // up stroke
        r.Inflate(0, -1);

        // get the path
        getRoundRectPath(&path, r, dia);

        // draw the round rect
        pGraphics->DrawPath(&pen, &path);
    }

    // restore page unit
    pGraphics->SetPageUnit((Unit)oldPageUnit);
}

void GdiPlusPainter::drawPause(int width, int height)
{
    Gdiplus::Font font(L"Arial Rounded MT", 50, FontStyleBold, UnitPixel);
    SolidBrush brush(Color::Black);

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);

    m_graphics.DrawString(L"Пауза", -1, &font, PointF(width / 2.0, height / 2.0), &format, &brush);
}

void GdiPlusPainter::drawHeartTimeLeft(int current, int total)
{
    Pen pen(Color::Red);
    pen.SetWidth(5);

    m_graphics.DrawArc(&pen, 0, 60, 25, 25, 90, 360 * static_cast<float>(current) / total);
}

void GdiPlusPainter::drawInvulnerabilityTimeLeft(int current, int total)
{
    Pen pen(Color::Orange);
    pen.SetWidth(5);

    m_graphics.DrawArc(&pen, 25, 60, 25, 25, 90, 360 * static_cast<float>(current) / total);
}