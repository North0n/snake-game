#pragma once

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