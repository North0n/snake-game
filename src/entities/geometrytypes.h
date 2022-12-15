#pragma once

#include <tuple>

struct Point
{
    int x;
    int y;

    Point() = default;

    Point(int x, int y)
        : x(x)
        , y(y)
    {
    }

    void moveOn(int x, int y)
    {
        this->x += x;
        this->y += y;
    }

    void moveTo(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    [[nodiscard]] bool isBetween(Point a, Point b) const
    {
        return (x >= a.x && x <= b.x) && (y == a.y && y == b.y) ||
               (y >= a.y && y <= b.y) && (x == a.x && x == b.x) ||
               (x >= b.x && x <= a.x) && (y == a.y && y == b.y) ||
               (y >= b.y && y <= a.y) && (x == a.x && x == b.x);
    }

    friend bool operator==(const Point& lhs, const Point& rhs)
    {
        return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
    }

    friend bool operator!=(const Point& lhs, const Point& rhs)
    {
        return !(rhs == lhs);
    }
};

struct Vector
{
    int x;
    int y;

    Vector(int x, int y)
        : x(x)
        , y(y)
    {
    }

    Vector operator*(int scalar) const
    {
        return {x * scalar, y * scalar};
    }
};